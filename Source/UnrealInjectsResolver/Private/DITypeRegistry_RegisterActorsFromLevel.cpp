#include "DITypeRegistry_RegisterActorsFromLevel.h"

#include "EngineUtils.h"

void UDITypeRegistry_RegisterActorsFromLevel::SetOuterContainer(TWeakObjectPtr<UDIContainer> Container)
{
}

void UDITypeRegistry_RegisterActorsFromLevel::RegisterType(const UDIRegisterType* RegisterType)
{
	const UDIRegisterType_RegisterActorsFromLevelBase* RegisterTypeObject
		= Cast<UDIRegisterType_RegisterActorsFromLevelBase>(RegisterType);
	if (!RegisterTypeObject)
	{
		UE_LOG(LogTemp, Error, TEXT("RegisterType: RegisterType is not UDIRegisterType_RegisterActorsFromLevelBase"));
		return;
	}

	ClassType = RegisterTypeObject->GetKeyClassType();
	ActorTag = RegisterTypeObject->GetActorTag();
	bNeedInject = RegisterTypeObject->GetNeedInject();
}

TTuple<UObject*, bool> UDITypeRegistry_RegisterActorsFromLevel::ResolveInstance(const UClass* KeyType)
{
	UObject* Object = nullptr;

	IterateActors(KeyType, [&Object](AActor* Actor)
	{
		Object = Actor;
		return false;
	});

	return {Object, bNeedInject};
}

TArray<TTuple<UObject*, bool>> UDITypeRegistry_RegisterActorsFromLevel::ResolveInstances(const UClass* KeyType)
{
	TArray<TTuple<UObject*, bool>> Objects;

	IterateActors(KeyType, [&Objects, this](AActor* Actor)
	{
		Objects.Add({Actor, bNeedInject});
		return true;
	});

	return Objects;
}

int UDITypeRegistry_RegisterActorsFromLevel::GetRegisterdTypeCount(const UClass* KeyType) const
{
	int Count = 0;

	IterateActors(KeyType, [&Count](AActor*)
	{
		++Count;
		return true;
	});

	return Count;
}

bool UDITypeRegistry_RegisterActorsFromLevel::IsSupported(const UDIRegisterType* RegisterType) const
{
	const UClass* BaseClass = UDIRegisterType_RegisterActorsFromLevelBase::StaticClass();
	return RegisterType->GetClass()->IsChildOf(BaseClass);
}

bool UDITypeRegistry_RegisterActorsFromLevel::IterateActors(const UClass* KeyType,
                                                            const TFunctionRef<bool(AActor*)>& Callback) const
{
	if (KeyType != ClassType)
	{
		return false;
	}

	if (!KeyType)
	{
		return false;
	}

	const UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	for (FActorIterator It(World); It; ++It)
	{
		if (AActor* Actor = *It; Actor->GetClass()->ImplementsInterface(KeyType))
		{
			if (ActorTag != NAME_None && !Actor->ActorHasTag(ActorTag))
			{
				continue;
			}

			if (!Callback(Actor))
			{
				return true;
			}
		}
	}

	return true;
}
