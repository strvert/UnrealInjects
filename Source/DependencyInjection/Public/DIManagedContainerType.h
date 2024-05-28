#pragma once
#include "CoreMinimal.h"
#include "DIManagedContainerType.generated.h"

UENUM()
enum class EDIManagedContainerType : uint8
{
	GameInstance,
	World,
};
