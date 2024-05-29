#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDITypeRegistry.generated.h"

class UDIContainer;
class UDIRegisterType;

UINTERFACE()
class UNREALINJECTS_API UDITypeRegistry : public UInterface
{
	GENERATED_BODY()
};

class UNREALINJECTS_API IDITypeRegistry
{
	GENERATED_BODY()

public:
	virtual void SetOuterContainer(TWeakObjectPtr<UDIContainer> Container) = 0;
	virtual void RegisterType(const UDIRegisterType* RegisterType) = 0;
	virtual TTuple<UObject*, bool> ResolveInstance(const UClass* KeyType) = 0;
	virtual TArray<TTuple<UObject*, bool>> ResolveInstances(const UClass* KeyType) = 0;
	virtual int GetRegisterdTypeCount(const UClass* KeyType) const = 0;
	virtual bool IsSupported(const UDIRegisterType* RegisterType) const = 0;
};

UCLASS(Abstract, EditInlineNew, HideCategories=Object, CollapseCategories)
class UNREALINJECTS_API UDIRegisterType : public UObject
{
	GENERATED_BODY()

public:
	virtual UClass* GetTypeRegistryClass() const { return nullptr; }
};
