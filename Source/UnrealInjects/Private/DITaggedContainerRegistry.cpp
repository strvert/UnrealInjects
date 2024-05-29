#include "DITaggedContainerRegistry.h"
#include "DILogChannels.h"

void UDITaggedContainerRegistry::Register(const FGameplayTag Tag, UDIContainer* Container)
{
	if (TaggedContainers.Contains(Tag))
	{
		UE_LOG(LogUnrealInjects, Warning, TEXT("Tagged container already exists for tag %s"), *Tag.ToString());
		return;
	}
	if (!Tag.IsValid())
	{
		UE_LOG(LogUnrealInjects, Warning, TEXT("Invalid tag for container"));
		return;
	}

	if (TaggedContainers.Contains(Tag))
	{
		UE_LOG(LogUnrealInjects, Warning, TEXT("Tagged container already exists for tag %s"), *Tag.ToString());
		return;
	}

	TaggedContainers.Add(Tag, Container);
}

void UDITaggedContainerRegistry::Unregister(const FGameplayTag Tag)
{
	TaggedContainers.Remove(Tag);
}

UDIContainer* UDITaggedContainerRegistry::FindContainer(const FGameplayTag Tag) const
{
	if (TaggedContainers.Contains(Tag))
	{
		return TaggedContainers[Tag];
	}

	return nullptr;
}
