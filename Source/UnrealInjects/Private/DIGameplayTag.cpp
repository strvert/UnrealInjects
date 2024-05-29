#include "DIGameplayTag.h"

namespace DITag
{
	namespace Container
	{
		UE_DEFINE_GAMEPLAY_TAG(Tag, "UnrealInjects.Container")
		namespace GameInstance
		{
			UE_DEFINE_GAMEPLAY_TAG(Tag, "UnrealInjects.Container.GameInstance")
		}
		namespace World
		{
			UE_DEFINE_GAMEPLAY_TAG(Tag, "UnrealInjects.Container.World")
		}
	}
}