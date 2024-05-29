#pragma once
#include "CoreMinimal.h"
#include "DIContainerLifetime.generated.h"

UENUM(BlueprintType)
enum class EDIContainerLifetime : uint8
{
	Transient,
	Scoped,
	Singleton
};
