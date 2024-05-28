#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DIManagedContainer.generated.h"

class UDITaggedContainerRegistry;
class UDIContainer;

USTRUCT()
struct FDIManagedContainerSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "/Script/DependencyInjection.DIContainerBuilderSettings"))
	FSoftObjectPath SettingAssetPath;
};

USTRUCT()
struct FDIManagedContainerSetting_World : public FDIManagedContainerSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (InlineEditConditionToggle))
	bool bUseParentContainer = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseParentContainer", Categories = "DependencyInjection.Container.GameInstance"))
	FGameplayTag ParentContainerTag;

	UPROPERTY(EditAnywhere, meta = (Categories = "DependencyInjection.Container.World"))
	FGameplayTag ContainerTag;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> AutoInjectActors;
};

USTRUCT()
struct FDIManagedContainerSetting_GameInstance : public FDIManagedContainerSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Categories = "DependencyInjection.Container.GameInstance"))
	FGameplayTag ContainerTag;
};


UCLASS(MinimalAPI, Config = Game, DefaultConfig, meta = (DisplayName = "Dependency Injection"))
class UDISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere)
	TArray<FDIManagedContainerSetting_GameInstance> ManagedContainersForGameInstance;
};


UCLASS()
class DEPENDENCYINJECTION_API UDIContainerGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY()
	TArray<TObjectPtr<UDIContainer>> ManagedContainers;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UDIContainer>> TaggedContainers;

	TArray<FGameplayTag> RegisteredTags;
	TWeakObjectPtr<UDITaggedContainerRegistry> TaggedContainerRegistry;
};


UCLASS(MinimalAPI)
class UDIWorldSettingUserData : public UAssetUserData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Dependency Injection")
	TArray<FDIManagedContainerSetting_World> ManagedContainers;
};


UCLASS()
class DEPENDENCYINJECTION_API UDIContainerWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	UPROPERTY()
	TArray<TObjectPtr<UDIContainer>> ManagedContainers;

	TWeakObjectPtr<UDITaggedContainerRegistry> TaggedContainerRegistry;
	TArray<FGameplayTag> RegisteredTags;
};
