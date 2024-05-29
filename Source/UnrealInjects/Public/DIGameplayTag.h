#pragma once
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

namespace DITag
{
	namespace Container
	{
		UNREALINJECTS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag);
		namespace GameInstance
		{
			UNREALINJECTS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag);
		}
		namespace World
		{
			UNREALINJECTS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tag);
		}
	}
}
