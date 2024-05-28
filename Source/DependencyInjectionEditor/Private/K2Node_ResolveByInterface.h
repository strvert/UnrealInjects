#pragma once

#include "CoreMinimal.h"
#include "K2Node_ConstructObjectFromClass.h"
#include "K2Node_ResolveByInterface.generated.h"

UCLASS()
class UK2Node_ResolveByInterface : public UK2Node_ConstructObjectFromClass
{
	GENERATED_BODY()

public:

	virtual void AllocateDefaultPins() override;
protected:
	virtual FText GetBaseNodeTitle() const override;
	virtual FText GetNodeTitleFormat() const override;
};
