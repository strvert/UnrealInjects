#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions/AssetTypeActions_DataAsset.h"
#include "Factories/Factory.h"
#include "DITypeRegistrationAssetFactory.generated.h"

UCLASS()
class UNREALINJECTSEDITOR_API UDITypeRegistrationAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UDITypeRegistrationAssetFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
};
