#pragma once
#include "CoreMinimal.h"
#include "DIContainerBuilderSettings.generated.h"

class UDIRegisterType;

UCLASS(BlueprintType, MinimalAPI)
class UDIContainerBuilderSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Dependency Injection", meta = (ShowOnlyInnerProperties))
	TArray<TObjectPtr<UDIRegisterType>> RegisterTypes;
};
