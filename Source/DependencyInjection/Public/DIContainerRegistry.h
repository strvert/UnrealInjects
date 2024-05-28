#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "IDITypeRegistry.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DIContainerRegistry.generated.h"

UCLASS()
class DEPENDENCYINJECTION_API UDIContainerRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void RegisterContainer(const FGameplayTag Tag, UDIContainer* Container);
	void UnregisterContainer(const FGameplayTag Tag);

private:

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UDIContainer>> TaggedContainers;
};
