#include "DIContainerRegistry.h"

void UDIContainerRegistry::RegisterContainer(const FGameplayTag Tag, UDIContainer* Container)
{
	TaggedContainers.Add(Tag, Container);
}

void UDIContainerRegistry::UnregisterContainer(const FGameplayTag Tag)
{
	TaggedContainers.Remove(Tag);
}
