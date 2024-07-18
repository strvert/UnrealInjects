#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DITypeRegistrationAsset.generated.h"

class UDIRegisterType;

UCLASS(BlueprintType, MinimalAPI)
class UDITypeRegistrationAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Dependency Injection", meta = (ShowOnlyInnerProperties))
	TArray<TObjectPtr<UDIRegisterType>> RegisterTypes;
};
