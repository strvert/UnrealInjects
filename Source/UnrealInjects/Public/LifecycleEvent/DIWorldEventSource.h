#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DIWorldEventSource.generated.h"

UCLASS()
class UNREALINJECTS_API UDIWorldEventSource : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;

	DECLARE_EVENT(UDIWorldEventSource, FPostInitialize)
	DECLARE_EVENT(UDIWorldEventSource, FOnWorldBeginPlay)
	DECLARE_EVENT_OneParam(UDIWorldEventSource, FOnWorldTick, float)

	FPostInitialize& OnPostInitializeEvent();
	FOnWorldBeginPlay& OnWorldBeginPlayEvent();
	FOnWorldTick& OnWorldTickEvent();

private:
	FPostInitialize PostInitializeEvent;
	FOnWorldBeginPlay WorldBeginPlayEvent;
	FOnWorldTick WorldTickEvent;
};
