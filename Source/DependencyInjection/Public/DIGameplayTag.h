#pragma once
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

namespace DITag
{
	namespace Container
	{
		DEPENDENCYINJECTION_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag);
		namespace GameInstance
		{
			DEPENDENCYINJECTION_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag);
		}
		namespace World
		{
			DEPENDENCYINJECTION_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag);
		}
	}
}
