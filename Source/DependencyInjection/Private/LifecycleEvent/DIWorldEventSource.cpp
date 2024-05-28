#include "LifecycleEvent/DIWorldEventSource.h"

void UDIWorldEventSource::OnWorldBeginPlay(UWorld& InWorld)
{
}

TStatId UDIWorldEventSource::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDIWorldEventProvider, STATGROUP_Tickables);
}

void UDIWorldEventSource::Tick(const float DeltaTime)
{
	WorldTickEvent.Broadcast(DeltaTime);
}

UDIWorldEventSource::FPostInitialize& UDIWorldEventSource::OnPostInitializeEvent()
{
	return PostInitializeEvent;
}

UDIWorldEventSource::FOnWorldBeginPlay& UDIWorldEventSource::OnWorldBeginPlayEvent()
{
	return WorldBeginPlayEvent;
}

UDIWorldEventSource::FOnWorldTick& UDIWorldEventSource::OnWorldTickEvent()
{
	return WorldTickEvent;
}
