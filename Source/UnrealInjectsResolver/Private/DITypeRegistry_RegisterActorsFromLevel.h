#pragma once
#include "CoreMinimal.h"

#include "UObject/Interface.h"
#include "Templates/SubclassOf.h"

#include "IDITypeRegistry.h"
#include "DITypeRegistry_RegisterActorsFromLevel.generated.h"

UCLASS()
class UDITypeRegistry_RegisterActorsFromLevel : public UObject, public IDITypeRegistry
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
	TObjectPtr<const UClass> ClassType;

	FName ActorTag;
	bool bNeedInject;

	bool IterateActors(const UClass* KeyType, const TFunctionRef<bool(AActor*)>& Callback) const;
};

UCLASS(Abstract)
class UDIRegisterType_RegisterActorsFromLevelBase : public UDIRegisterType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "")
	FName ActorTag;

	UPROPERTY(EditAnywhere, Category = "")
	bool bNeedInject;

	virtual UClass* GetTypeRegistryClass() const override
	{
		return UDITypeRegistry_RegisterActorsFromLevel::StaticClass();
	}

	virtual UClass* GetKeyClassType() const { return nullptr; }

	FName GetActorTag() const { return ActorTag; }

	bool GetNeedInject() const { return bNeedInject; }
};

UCLASS(DisplayName = "Register Actors With Class From Level")
class UDIRegisterType_RegisterActorsWithClassFromLevel : public UDIRegisterType_RegisterActorsFromLevelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "")
	TObjectPtr<UClass> ClassType;

	virtual UClass* GetKeyClassType() const override
	{
		return ClassType.Get();
	}
};

UCLASS(DisplayName = "Register Actors With Interface From Level")
class UDIRegisterType_RegisterActorsWithInterfaceFromLevel : public UDIRegisterType_RegisterActorsFromLevelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "", meta = (AllowAbstract))
	TSubclassOf<UInterface> InterfaceType;

	virtual UClass* GetKeyClassType() const override
	{
		return InterfaceType.Get();
	}
};
