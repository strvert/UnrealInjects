#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDICleanup.generated.h"


UINTERFACE(NotBlueprintable)
class UDIEventCleanup : public UInterface
{
	GENERATED_BODY()
};

class DEPENDENCYINJECTION_API IDIEventCleanup
{
	GENERATED_BODY()

public:
	virtual void DI_Cleanup() = 0;
};


UINTERFACE(Blueprintable, DisplayName="DI Event Cleanup")
class UDIEventCleanupBP : public UInterface
{
	GENERATED_BODY()
};

class DEPENDENCYINJECTION_API IDIEventCleanupBP
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Cleanup")
	void DI_Cleanup();
};
