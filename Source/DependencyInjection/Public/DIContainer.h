#pragma once

#include "CoreMinimal.h"
#include "DIContainerBuilderSettings.h"
#include "DIContainerLifetime.h"
#include "DIContainer.generated.h"

class IDITypeRegistry;

UCLASS(BlueprintType)
class DEPENDENCYINJECTION_API UDIContainer : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Dependency Injection")
	UObject* ResolveInstance(const UClass* Type);

	UFUNCTION(BlueprintCallable, Category = "Dependency Injection")
	TArray<UObject*> ResolveInstances(const UClass* Type);

	UFUNCTION(BlueprintCallable, Category = "Dependency Injection")
	void SetParentContainer(UDIContainer* Parent);

	UFUNCTION(BlueprintCallable, Category = "Dependency Injection")
	bool InjectDependencies(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Dependency Injection")
	void Cleanup();
	
	UObject* FindResolvedInstance(const FName Key, const EDIContainerLifetime Lifetime);
	void AddResolvedInstance(const FName Key, UObject* Object, const EDIContainerLifetime Lifetime);

private:
	UObject* ResolveInternal(const UClass* Type);
	TArray<UObject*> ResolveAllInternal(const UClass* Type);

	void AddType(const UDIRegisterType* RegisterType);

	bool InjectDependenciesToMethods(UObject* Object);
	bool InjectMethodArguments(UObject* Object, UFunction* Method);
	bool InjectDependenciesToProperties(UObject* Object);

	void InjectLifecycleEvents(UObject* Object, const UClass* KeyType);
	void InjectCommonLifecycleEvents(UObject* Object, const UClass* KeyType);
	void InjectGameInstanceLifecycleEvents(UObject* Object, const UClass* KeyType);
	void InjectWorldLifecycleEvents(UObject* Object, const UClass* KeyType);

	bool ResolveAndSetField(void* Container, const FProperty* Property, void* Resolved = nullptr);
	bool ResolveAndSetArrayField(const void* Container, const FArrayProperty* Property);
	bool ResolveAndSetInterfaceField(void* Container, const FInterfaceProperty* Property, void* Resolved = nullptr);
	bool ResolveAndSetClassField(void* Container, const FObjectProperty* Property, void* Resolved = nullptr);
	
	int GetTotalRegisterdTypeCount(const UClass* Class);

	static UClass* TryGetInterfaceOrClassType(const FProperty* Property);

	UPROPERTY()
	TMap<TObjectPtr<UClass>, TScriptInterface<IDITypeRegistry>> TypeRegistries;

	UPROPERTY()
	TMap<FName, TObjectPtr<UObject>> ResolvedInstances;

	UPROPERTY()
	UDIContainer* ParentContainer;

	friend class UDIContainerBuilder;
};
