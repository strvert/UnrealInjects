#include "DITypeRegistry_Object.h"

#include "DIContainer.h"
#include "DIResolverLogChannels.h"

void UDITypeRegistry_Object::SetOuterContainer(TWeakObjectPtr<UDIContainer> Container)
{
	OuterContainer = Container;
}

void UDITypeRegistry_Object::RegisterType(const UDIRegisterType* RegisterType)
{
	const UDIRegisterType_Object* RegisterTypeObject = Cast<UDIRegisterType_Object>(RegisterType);
	if (!RegisterTypeObject)
	{
		UE_LOG(LogUnrealInjectsResolver, Error, TEXT("RegisterType: RegisterType is not UDIRegisterType_Object"));
		return;
	}

	if (const UClass* ClassType = RegisterTypeObject->ClassType)
	{
		for (const FImplementedInterface& ImplementedInterface : ClassType->Interfaces)
		{
			const UClass* Interface = ImplementedInterface.Class;
			RegisteredTypes.FindOrAdd(Interface).Values.Add({ClassType, RegisterTypeObject->Lifetime});
		}
		RegisteredTypes.FindOrAdd(ClassType).Values.Add({ClassType, RegisterTypeObject->Lifetime});
	}
}

TTuple<UObject*, bool> UDITypeRegistry_Object::ResolveInstance(const UClass* KeyType)
{
	if (const FTypeRecords* Records = RegisteredTypes.Find(KeyType))
	{
		const auto& [ClassType, Lifetime] = Records->Values.Last();

		if (UObject* Object = FindInstance(ClassType, Lifetime))
		{
			return {Object, false};
		}

		if (UObject* Object = CreateObjectByLifetime(ClassType, Lifetime))
		{
			AddInstance(ClassType, Object, Lifetime);
			return {Object, true};
		}
	}

	return {nullptr, false};
}

TArray<TTuple<UObject*, bool>> UDITypeRegistry_Object::ResolveInstances(const UClass* KeyType)
{
	TArray<TTuple<UObject*, bool>> Objects;

	if (const FTypeRecords* Records = RegisteredTypes.Find(KeyType))
	{
		for (const auto& [ClassType, Lifetime] : Records->Values)
		{
			if (UObject* Object = FindInstance(ClassType, Lifetime))
			{
				Objects.Add({Object, false});
				continue;
			}
			
			if (UObject* Object = CreateObjectByLifetime(ClassType, Lifetime))
			{
				AddInstance(ClassType, Object, Lifetime);
				Objects.Add({Object, true});
			}
		}
	}

	return Objects;
}

int UDITypeRegistry_Object::GetRegisterdTypeCount(const UClass* Type) const
{
	if (const FTypeRecords* Records = RegisteredTypes.Find(Type))
	{
		return Records->Values.Num();
	}

	return 0;
}

bool UDITypeRegistry_Object::IsSupported(const UDIRegisterType* RegisterType) const
{
	return RegisterType->GetClass() == UDIRegisterType_Object::StaticClass();
}

UObject* UDITypeRegistry_Object::CreateObjectByLifetime(const UClass* ClassType,
                                                        const EDIContainerLifetime Lifetime) const
{
	if (OuterContainer.IsValid())
	{
		if (Lifetime == EDIContainerLifetime::Transient)
		{
			return NewObject<UObject>(OuterContainer.Get(), ClassType);
		}

		if (Lifetime == EDIContainerLifetime::Scoped || Lifetime == EDIContainerLifetime::Singleton)
		{
			UObject* Object = NewObject<UObject>(OuterContainer.Get(), ClassType);
			return Object;
		}
	}

	return nullptr;
}

UObject* UDITypeRegistry_Object::FindInstance(const UClass* ClassType, const EDIContainerLifetime Lifetime) const
{
	if (OuterContainer.IsValid())
	{
		return OuterContainer->FindResolvedInstance(ClassType->GetFName(), Lifetime);
	}

	return nullptr;
}

void UDITypeRegistry_Object::AddInstance(const UClass* ClassType, UObject* Object,
                                         const EDIContainerLifetime Lifetime) const
{
	if (Lifetime == EDIContainerLifetime::Transient)
	{
		return;
	}

	if (OuterContainer.IsValid())
	{
		OuterContainer->AddResolvedInstance(ClassType->GetFName(), Object, Lifetime);
	}
}
