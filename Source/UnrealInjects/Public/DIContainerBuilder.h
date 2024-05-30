#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "DIContainerBuilder.generated.h"

class UDIContainer;
class UDITypeRegistrationAsset;

UCLASS(BlueprintType)
class UNREALINJECTS_API UDIContainerBuilder : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Dependency Injection", meta = (WorldContext = "WorldContextObject"))
	static UDIContainerBuilder* MakeContainerBuilder(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Dependency Injection")
	UDIContainer* BuildContainer();

	UFUNCTION(BlueprintCallable, Category = "Dependency Injection")
	void RegisterTypesByAsset(UDITypeRegistrationAsset* InSettingsAsset);

	UFUNCTION(BlueprintCallable, Category = "Dependency Injection", meta = (GameplayTagFilter = "UnrealInjects.Container"))
	void SetParentContainerTag(const FGameplayTag InParentContainerTag);

	UFUNCTION(BlueprintCallable, Category = "Dependency Injection")
	void RegisterAutoInjectObject(UObject* Object);

private:
	static void InitialResolve(UDIContainer* Container);
	static void RegisterTypesBySettingsToContainer(UDIContainer* Container, UDITypeRegistrationAsset* Settings);

	UPROPERTY()
	TObjectPtr<UDITypeRegistrationAsset> Settings;

	UPROPERTY()
	TArray<TObjectPtr<UObject>> AutoInjectObjects;

	FGameplayTag ParentContainerTag;
};
