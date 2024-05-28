#include "DIGameplayTag.h"

namespace DITag
{
	namespace Container
	{
		UE_DEFINE_GAMEPLAY_TAG(Tag, "DependencyInjection.Container")
		namespace GameInstance
		{
			UE_DEFINE_GAMEPLAY_TAG(Tag, "DependencyInjection.Container.GameInstance")
		}
		namespace World
		{
			UE_DEFINE_GAMEPLAY_TAG(Tag, "DependencyInjection.Container.World")
		}
	}
}