#include "DIManagedContainer.h"

#include "DIContainer.h"
#include "DIContainerBuilder.h"
#include "DITypeRegistrationAsset.h"
#include "DITaggedContainerRegistry.h"

void BuildAndRegisterContainer(UDIContainerBuilder* Builder,
                               UDITaggedContainerRegistry& TaggedContainerRegistry,
                               const FSoftObjectPath& TypeRegistrationAssetPath,
                               const FGameplayTag& ParentContainerTag,
                               const FGameplayTag& ContainerTag,
                               TArray<TObjectPtr<UDIContainer>>& ManagedContainers)
{
	if (UDITypeRegistrationAsset* SettingAsset = Cast<UDITypeRegistrationAsset>(TypeRegistrationAssetPath.TryLoad()))
	{
		Builder->RegisterTypesByAsset(SettingAsset);
		Builder->SetParentContainerTag(ParentContainerTag);
		if (UDIContainer* Container = Builder->BuildContainer())
		{
			ManagedContainers.Add(Container);
			if (ContainerTag.IsValid())
			{
				TaggedContainerRegistry.Register(ContainerTag, Container);
			}
		}
	}
}

void UDIContainerGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	TaggedContainerRegistry
		= Cast<UDITaggedContainerRegistry>(Collection.InitializeDependency(UDITaggedContainerRegistry::StaticClass()));
	check(TaggedContainerRegistry.IsValid());

	for (const UDISettings* Settings = GetDefault<UDISettings>();
	     const FDIManagedContainerSetting_GameInstance& ContainerSetting : Settings->ManagedContainersForGameInstance)
	{
		const FSoftObjectPath& TypeRegistrationAssetPath = ContainerSetting.TypeRegistrationAssetPath;
		const FGameplayTag& ContainerTag = ContainerSetting.ContainerTag;

		UDIContainerBuilder* Builder = UDIContainerBuilder::MakeContainerBuilder(this);
		BuildAndRegisterContainer(Builder, *TaggedContainerRegistry,
		                          TypeRegistrationAssetPath, FGameplayTag::EmptyTag, ContainerTag,
		                          ManagedContainers);

		if (ContainerTag.IsValid())
		{
			RegisteredTags.Add(ContainerTag);
		}
	}
}

void UDIContainerGameInstanceSubsystem::Deinitialize()
{
	if (TaggedContainerRegistry.IsValid())
	{
		for (const FGameplayTag& Tag : RegisteredTags)
		{
			TaggedContainerRegistry->Unregister(Tag);
		}
	}

	for (UDIContainer* Container : ManagedContainers)
	{
		Container->Cleanup();
	}
}

void UDIContainerWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	AWorldSettings* WorldSettings = InWorld.GetWorldSettings();
	check(WorldSettings);

	const UGameInstance* GameInstance = InWorld.GetGameInstance();
	TaggedContainerRegistry
		= Cast<UDITaggedContainerRegistry>(GameInstance->GetSubsystem<UDITaggedContainerRegistry>());
	check(TaggedContainerRegistry.IsValid());

	UDIWorldSettingUserData* UserData
		= Cast<UDIWorldSettingUserData>(WorldSettings->GetAssetUserDataOfClass(UDIWorldSettingUserData::StaticClass()));

	if (!UserData)
	{
		return;
	}

	for (const FDIManagedContainerSetting_World& ContainerSetting : UserData->ManagedContainers)
	{
		const FSoftObjectPath& TypeRegistrationAssetPath = ContainerSetting.TypeRegistrationAssetPath;
		const FGameplayTag& ParentContainerTag = ContainerSetting.ParentContainerTag;
		const FGameplayTag& ContainerTag = ContainerSetting.ContainerTag;
		const TArray<TObjectPtr<AActor>>& Actors = ContainerSetting.AutoInjectActors;

		UDIContainerBuilder* Builder = UDIContainerBuilder::MakeContainerBuilder(this);
		for (AActor* Actor : Actors)
		{
			Builder->RegisterAutoInjectObject(Actor);
		}
		BuildAndRegisterContainer(Builder, *TaggedContainerRegistry,
		                          TypeRegistrationAssetPath, ParentContainerTag, ContainerTag,
		                          ManagedContainers);

		if (ContainerTag.IsValid())
		{
			RegisteredTags.Add(ContainerTag);
		}
	}
}

void UDIContainerWorldSubsystem::Deinitialize()
{
	if (TaggedContainerRegistry.IsValid())
	{
		for (const FGameplayTag& Tag : RegisteredTags)
		{
			TaggedContainerRegistry->Unregister(Tag);
		}
	}

	for (UDIContainer* Container : ManagedContainers)
	{
		Container->Cleanup();
	}
}

bool UDIContainerWorldSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}
