#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDITick.generated.h"

UINTERFACE(NotBlueprintable)
class UDIEventTick : public UInterface
{
	GENERATED_BODY()
};

class UNREALINJECTS_API IDIEventTick
{
	GENERATED_BODY()

public:
	virtual void DI_Tick(float DeltaTime) = 0;
};


UINTERFACE(Blueprintable, DisplayName="DI Event Tick")
class UDIEventTickBP : public UInterface
{
	GENERATED_BODY()
};

class UNREALINJECTS_API IDIEventTickBP
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Tick")
	void DI_Tick(float DeltaTime);
};
