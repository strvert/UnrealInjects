#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDIConstruct.generated.h"


UINTERFACE(NotBlueprintable)
class UDIEventConstruct : public UInterface
{
	GENERATED_BODY()
};

class DEPENDENCYINJECTION_API IDIEventConstruct
{
	GENERATED_BODY()

public:
	virtual void DI_Construct() = 0;
};


UINTERFACE(Blueprintable, DisplayName="DI Event Construct")
class UDIEventConstructBP : public UInterface
{
	GENERATED_BODY()
};

class DEPENDENCYINJECTION_API IDIEventConstructBP
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Construct")
	void DI_Construct();
};
