#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "DITaggedContainerRegistry.generated.h"

class UDIContainer;

UCLASS()
class UNREALINJECTS_API UDITaggedContainerRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void Register(const FGameplayTag Tag, UDIContainer* Container);
	void Unregister(const FGameplayTag Tag);

	UDIContainer* FindContainer(const FGameplayTag Tag) const;

private:
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UDIContainer>> TaggedContainers;
};
