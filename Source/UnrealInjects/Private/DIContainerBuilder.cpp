#include "DIContainerBuilder.h"

#include "DITypeRegistrationAsset.h"
#include "DIContainer.h"
#include "DILogChannels.h"
#include "DITaggedContainerRegistry.h"
#include "LifecycleEvent/Common/IDIConstruct.h"
#include "LifecycleEvent/World/IDITick.h"

UDIContainerBuilder* UDIContainerBuilder::MakeContainerBuilder(UObject* WorldContextObject)
{
	UDIContainerBuilder* ContainerBuilder = NewObject<UDIContainerBuilder>(WorldContextObject);

	return ContainerBuilder;
}

UDIContainer* UDIContainerBuilder::BuildContainer()
{
	UDIContainer* Container = NewObject<UDIContainer>(this);

	if (const UWorld* World = GetWorld(); World && ParentContainerTag.IsValid())
	{
		const UGameInstance* GameInstance = World->GetGameInstance();
		check(GameInstance);
		const UDITaggedContainerRegistry* TaggedContainerRegistry
			= GameInstance->GetSubsystem<UDITaggedContainerRegistry>();
		check(TaggedContainerRegistry);

		if (UDIContainer* TaggedContainer = TaggedContainerRegistry->FindContainer(ParentContainerTag))
		{
			Container->SetParentContainer(TaggedContainer);
		}
		else
		{
			UE_LOG(LogUnrealInjects, Warning,
			       TEXT("The parent container tag %s was set but the container was not found."),
			       *ParentContainerTag.ToString());
		}
	}

	if (Settings)
	{
		RegisterTypesBySettingsToContainer(Container, Settings);
	}

	InitialResolve(Container);

	for (UObject* Object : AutoInjectObjects)
	{
		Container->InjectDependencies(Object);
	}

	return Container;
}

void UDIContainerBuilder::RegisterTypesByAsset(UDITypeRegistrationAsset* InSettingsAsset)
{
	Settings = InSettingsAsset;
}

void UDIContainerBuilder::SetParentContainerTag(const FGameplayTag InParentContainerTag)
{
	ParentContainerTag = InParentContainerTag;
}

void UDIContainerBuilder::RegisterAutoInjectObject(UObject* Object)
{
	AutoInjectObjects.Add(Object);
}

void UDIContainerBuilder::InitialResolve(UDIContainer* Container)
{
	Container->ResolveInstances(UDIEventConstruct::StaticClass());
	Container->ResolveInstances(UDIEventConstructBP::StaticClass());

	Container->ResolveInstances(UDIEventTick::StaticClass());
	Container->ResolveInstances(UDIEventTickBP::StaticClass());
}

void UDIContainerBuilder::RegisterTypesBySettingsToContainer(UDIContainer* Container,
                                                             UDITypeRegistrationAsset* Settings)
{
	for (const TObjectPtr<UDIRegisterType>& RegisterType : Settings->RegisterTypes)
	{
		Container->AddType(RegisterType);
	}
}
