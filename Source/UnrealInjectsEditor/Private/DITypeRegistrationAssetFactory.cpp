#include "DITypeRegistrationAssetFactory.h"

#include "DITypeRegistrationAsset.h"

UDITypeRegistrationAssetFactory::UDITypeRegistrationAssetFactory()
{
	SupportedClass = UDITypeRegistrationAsset::StaticClass();

	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UDITypeRegistrationAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
                                                              EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	ensure(UDITypeRegistrationAsset::StaticClass() == InClass);

	UDITypeRegistrationAsset* NewSettings = NewObject<UDITypeRegistrationAsset>(InParent, InClass, InName, Flags);

	return NewSettings;
}
