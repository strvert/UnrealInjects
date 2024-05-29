#include "DIContainer.h"

#include "DILogChannels.h"
#include "IDITypeRegistry.h"
#include "EntitySystem/MovieSceneComponentDebug.h"
#include "LifecycleEvent/DIWorldEventSource.h"
#include "LifecycleEvent/Common/IDICleanup.h"
#include "LifecycleEvent/Common/IDIConstruct.h"
#include "LifecycleEvent/World/IDITick.h"

class UDIWorldEventSource;

UObject* UDIContainer::ResolveInstance(const UClass* Type)
{
	return ResolveInternal(Type);
}

TArray<UObject*> UDIContainer::ResolveInstances(const UClass* Type)
{
	return ResolveAllInternal(Type);
}

void UDIContainer::SetParentContainer(UDIContainer* Parent)
{
	ParentContainer = Parent;
}

UObject* UDIContainer::ResolveInternal(const UClass* Type)
{
	for (const auto& [_, TypeRegistry] : TypeRegistries)
	{
		if (auto [Resolved, bIsNeedInject] = TypeRegistry->ResolveInstance(Type); Resolved)
		{
			if (bIsNeedInject)
			{
				InjectDependencies(Resolved);
				InjectLifecycleEvents(Resolved, Type);
			}
			return Resolved;
		}
	}

	if (ParentContainer)
	{
		return ParentContainer->ResolveInternal(Type);
	}

	return nullptr;
}

TArray<UObject*> UDIContainer::ResolveAllInternal(const UClass* Type)
{
	TArray<UObject*> Objects;

	for (const auto& [_, TypeRegistry] : TypeRegistries)
	{
		TArray<TTuple<UObject*, bool>> Resolved = TypeRegistry->ResolveInstances(Type);
		Objects.Reserve(Objects.Num() + Resolved.Num());
		for (const auto [Object, bIsNeedInject] : Resolved)
		{
			if (Object)
			{
				if (bIsNeedInject)
				{
					InjectDependencies(Object);
					InjectLifecycleEvents(Object, Type);
				}
				Objects.Add(Object);
			}
		}
	}

	if (ParentContainer)
	{
		Objects.Append(ParentContainer->ResolveAllInternal(Type));
	}

	return Objects;
}

UObject* UDIContainer::FindResolvedInstance(const FName Key, const EDIContainerLifetime Lifetime)
{
	if (Lifetime == EDIContainerLifetime::Transient)
	{
		return nullptr;
	}

	if (TObjectPtr<UObject>* Resolved = ResolvedInstances.Find(Key))
	{
		return *Resolved;
	}

	if (ParentContainer && Lifetime == EDIContainerLifetime::Singleton)
	{
		return ParentContainer->FindResolvedInstance(Key, Lifetime);
	}

	return nullptr;
}

void UDIContainer::AddResolvedInstance(const FName Key, UObject* Object, const EDIContainerLifetime Lifetime)
{
	if (Lifetime == EDIContainerLifetime::Transient)
	{
		return;
	}

	if (Lifetime == EDIContainerLifetime::Singleton && ParentContainer)
	{
		ParentContainer->AddResolvedInstance(Key, Object, Lifetime);
		return;
	}

	ResolvedInstances.Add(Key, Object);
}

void UDIContainer::AddType(const UDIRegisterType* RegisterType)
{
	UClass* RegistryClass = RegisterType->GetTypeRegistryClass();

	TScriptInterface<IDITypeRegistry>& Registry = TypeRegistries.FindOrAdd(RegistryClass);

	if (!Registry.GetObject())
	{
		Registry = NewObject<UObject>(this, RegistryClass);
		Registry->SetOuterContainer(this);
	}

	Registry->RegisterType(RegisterType);
}

bool UDIContainer::InjectDependencies(UObject* Object)
{
	return InjectDependenciesToMethods(Object) && InjectDependenciesToProperties(Object);
}

void UDIContainer::Cleanup()
{
	for (auto& [Name, Object] : ResolvedInstances)
	{
		if (IDIEventCleanup* CleanupEvent = Cast<IDIEventCleanup>(Object))
		{
			CleanupEvent->DI_Cleanup();
		}
		else if (Object->Implements<UDIEventCleanupBP>())
		{
			IDIEventCleanupBP::Execute_DI_Cleanup(Object);
		}
		
		// Object->ConditionalBeginDestroy();
	}
}

bool UDIContainer::InjectDependenciesToMethods(UObject* Object)
{
	if (!Object)
	{
		UE_LOG(LogUnrealInjects, Error, TEXT("InjectDependenciesToMethods: Object is nullptr"));
		return false;
	}
	const UClass* ClassType = Object->GetClass();

	TArray<FName> FunctionNames;
	ClassType->GenerateFunctionList(FunctionNames);

	static FName InjectTag = FName(TEXT("InjectDependencies"));

	for (const FName& FunctionName : FunctionNames)
	{
		UFunction* Function = ClassType->FindFunctionByName(FunctionName);

		if (!Function)
		{
			continue;
		}

		const bool bIsNative = Function->IsNative();
		if ((bIsNative && Function->HasMetaData(InjectTag)) ||
			(!bIsNative && FunctionName == InjectTag))
		{
			UE_LOG(LogUnrealInjects, Log, TEXT("Injecting %s"), *Function->GetName());
			if (!InjectMethodArguments(Object, Function))
			{
				UE_LOG(LogUnrealInjects, Error, TEXT("Failed to inject method arguments (%s)"), *Function->GetName());
				return false;
			}
		}
	}
	return true;
}

bool UDIContainer::InjectMethodArguments(UObject* Object, UFunction* Method)
{
	TArray<uint8, TInlineAllocator<32>> Args;
	uint8* ArgData = Args.GetData();

	Args.SetNumZeroed(Method->ParmsSize);

	// Iterate Arguments
	for (TFieldIterator<FProperty> PropIt(Method); PropIt && (PropIt->PropertyFlags & CPF_Parm); ++PropIt)
	{
		if (!ResolveAndSetField(ArgData, *PropIt))
		{
			UE_LOG(LogUnrealInjects, Error, TEXT("Failed to resolve and set field (%s)"), *PropIt->GetName());
			return false;
		}
	}

	// Call inject function
	Object->ProcessEvent(Method, Args.GetData());

	return true;
}

bool UDIContainer::InjectDependenciesToProperties(UObject* Object)
{
	const UClass* ClassType = Object->GetClass();

	for (TFieldIterator<FProperty> PropIt(ClassType); PropIt; ++PropIt)
	{
		if (const FProperty* Property = *PropIt; Property->HasMetaData("Inject"))
		{
			if (!ResolveAndSetField(Object, Property))
			{
				UE_LOG(LogUnrealInjects, Error, TEXT("Failed to resolve and set field (%s)"), *Property->GetName());
				return false;
			}
		}
	}
	return true;
}

void UDIContainer::InjectLifecycleEvents(UObject* Object, const UClass* KeyType)
{
	const UClass* ClassType = Object->GetClass();
	if (ClassType->IsChildOf<AActor>())
	{
		return;
	}

	InjectCommonLifecycleEvents(Object, KeyType);
	InjectGameInstanceLifecycleEvents(Object, KeyType);
	InjectWorldLifecycleEvents(Object, KeyType);
}

void UDIContainer::InjectCommonLifecycleEvents(UObject* Object, const UClass* KeyType)
{
	const UClass* ClassType = Object->GetClass();
	if (UDIEventConstruct::StaticClass() == KeyType)
	{
		if (IDIEventConstruct* ConstructEvent = Cast<IDIEventConstruct>(Object))
		{
			ConstructEvent->DI_Construct();
		}
		return;
	}

	if (UDIEventConstructBP::StaticClass() == KeyType)
	{
		if (ClassType->ImplementsInterface(UDIEventConstructBP::StaticClass()))
		{
			IDIEventConstructBP::Execute_DI_Construct(Object);
		}
		return;
	}
}

void UDIContainer::InjectGameInstanceLifecycleEvents(UObject* Object, const UClass* KeyType)
{
}

void UDIContainer::InjectWorldLifecycleEvents(UObject* Object, const UClass* KeyType)
{
	const UWorld* World = GetWorld();
	const UClass* ClassType = Object->GetClass();

	if (!World)
	{
		return;
	}

	UDIWorldEventSource* EventProvider = World->GetSubsystem<UDIWorldEventSource>();

	if (!EventProvider)
	{
		return;
	}

	// Tick
	if (UDIEventTick::StaticClass() == KeyType)
	{
		if (IDIEventTick* TickEvent = Cast<IDIEventTick>(Object))
		{
			EventProvider->OnWorldTickEvent().AddRaw(TickEvent, &IDIEventTick::DI_Tick);
		}
		return;
	}

	if (UDIEventTickBP::StaticClass() == KeyType)
	{
		if (ClassType->ImplementsInterface(UDIEventTickBP::StaticClass()))
		{
			EventProvider->OnWorldTickEvent().AddLambda([Object](const float DeltaTime)
			{
				IDIEventTickBP::Execute_DI_Tick(Object, DeltaTime);
			});
		}
		return;
	}
}

bool UDIContainer::ResolveAndSetField(void* Container, const FProperty* Property, void* Resolved)
{
	if (const FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
	{
		return ResolveAndSetArrayField(Container, ArrayProperty);
	}
	if (const FInterfaceProperty* InterfaceProperty = CastField<FInterfaceProperty>(Property))
	{
		return ResolveAndSetInterfaceField(Container, InterfaceProperty, Resolved);
	}
	if (const FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
	{
		return ResolveAndSetClassField(Container, ObjectProperty);
	}

	UE_LOG(LogUnrealInjects, Error, TEXT("Property (%s) is not supported"), *Property->GetName());

	return false;
}

bool UDIContainer::ResolveAndSetArrayField(const void* Container, const FArrayProperty* Property)
{
	// Array Field
	const UClass* InnerClass = TryGetInterfaceOrClassType(Property->Inner);
	if (!InnerClass)
	{
		UE_LOG(LogUnrealInjects, Error, TEXT("Array Property (%s) has no Inner Class"), *Property->GetName());
		return false;
	}

	const void* ArrayData = Property->ContainerPtrToValuePtr<void>(Container);
	FScriptArrayHelper ArrayHelper(Property, ArrayData);
	TArray<UObject*> ResolvedObjects = ResolveAllInternal(InnerClass);

	if (ResolvedObjects.Num() == 0)
	{
		UE_LOG(LogUnrealInjects, Error, TEXT("Failed to resolve Array Field (%s)"), *Property->GetName());
		return false;
	}

	ArrayHelper.EmptyAndAddValues(ResolvedObjects.Num());

	for (int Index = 0; Index < ResolvedObjects.Num(); ++Index)
	{
		if (!ResolveAndSetField(ArrayHelper.GetRawPtr(Index), Property->Inner, ResolvedObjects[Index]))
		{
			UE_LOG(LogUnrealInjects, Error, TEXT("Failed to resolve Array Field (%s)"), *Property->GetName());
			return false;
		}
	}

	return true;
}

bool UDIContainer::ResolveAndSetInterfaceField(void* Container, const FInterfaceProperty* Property, void* Resolved)
{
	UObject* ResolvedObject = static_cast<UObject*>(Resolved);
	if (!ResolvedObject)
	{
		ResolvedObject = ResolveInternal(Property->InterfaceClass);
	}

	if (!ResolvedObject)
	{
		UE_LOG(LogUnrealInjects, Error, TEXT("Failed to resolve Interface (%s)"), *Property->GetName());
		return false;
	}

	void* InterfaceAddress = ResolvedObject->GetInterfaceAddress(Property->InterfaceClass);
	Property->SetValue_InContainer(Container, FScriptInterface(ResolvedObject, InterfaceAddress));

	return true;
}

bool UDIContainer::ResolveAndSetClassField(void* Container, const FObjectProperty* Property, void* Resolved)
{
	UObject* ResolvedObject = static_cast<UObject*>(Resolved);
	if (!ResolvedObject)
	{
		ResolvedObject = ResolveInternal(Property->PropertyClass);
	}

	if (!ResolvedObject)
	{
		UE_LOG(LogUnrealInjects, Error, TEXT("Failed to resolve Class (%s)"), *Property->GetName());
		return false;
	}

	Property->SetValue_InContainer(Container, ResolvedObject);

	return true;
}

int UDIContainer::GetTotalRegisterdTypeCount(const UClass* Class)
{
	int Count = 0;
	for (const auto& [_, TypeRegistry] : TypeRegistries)
	{
		Count += TypeRegistry->GetRegisterdTypeCount(Class);
	}
	return Count;
}

UClass* UDIContainer::TryGetInterfaceOrClassType(const FProperty* Property)
{
	if (const FInterfaceProperty* InterfaceProperty = CastField<FInterfaceProperty>(Property))
	{
		return InterfaceProperty->InterfaceClass;
	}

	if (const FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
	{
		return ObjectProperty->PropertyClass;
	}

	return nullptr;
}
