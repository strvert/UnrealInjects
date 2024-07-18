#pragma once

#include "CoreMinimal.h"
#include "DIContainer.h"
#include "IDITypeRegistry.h"
#include "DITypeRegistry_Object.generated.h"

USTRUCT()
struct FTypeRecordValue
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<const UClass> ClassType;

	EDIContainerLifetime Lifetime;
};

USTRUCT()
struct FTypeRecords
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FTypeRecordValue> Values;
};

USTRUCT()
struct FPropertyDependency
{
	GENERATED_BODY()

	TArray<const FProperty*> Properties;
};

UCLASS()
class UDITypeRegistry_Object : public UObject, public IDITypeRegistry
{
	GENERATED_BODY()

public:
	virtual void SetOuterContainer(TWeakObjectPtr<UDIContainer> Container) override;
	virtual void RegisterType(const UDIRegisterType* RegisterType) override;
	virtual TTuple<UObject*, bool> ResolveInstance(const UClass* KeyType) override;
	virtual TArray<TTuple<UObject*, bool>> ResolveInstances(const UClass* KeyType) override;
	virtual int GetRegisterdTypeCount(const UClass* KeyType) const override;
	virtual bool IsSupported(const UDIRegisterType* RegisterType) const override;
	
private:
	UPROPERTY()
	TMap<TObjectPtr<const UClass>, FTypeRecords> RegisteredTypes;

	TWeakObjectPtr<UDIContainer> OuterContainer;
	
	UObject* CreateObjectByLifetime(const UClass* ClassType, const EDIContainerLifetime Lifetime) const;

	UObject* FindInstance(const UClass* ClassType, const EDIContainerLifetime Lifetime) const;
	void AddInstance(const UClass* ClassType, UObject* Object, const EDIContainerLifetime Lifetime) const;
};

UCLASS(DisplayName = "Register Object Class")
class UDIRegisterType_Object : public UDIRegisterType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "")
	TSubclassOf<UObject> ClassType;

	UPROPERTY(EditAnywhere, Category = "")
	EDIContainerLifetime Lifetime = EDIContainerLifetime::Scoped;

	virtual UClass* GetTypeRegistryClass() const override
	{
		return UDITypeRegistry_Object::StaticClass();
	}
};
