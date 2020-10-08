// Made by Alex Jobe


#include "PowerupActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APowerupActor::APowerupActor()
{
	PowerupInterval = 0.f;
	TotalTicks = 0;
	TicksProcessed = 0;

	bIsPowerupActive = false;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void APowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void APowerupActor::ActivatePowerup(AActor* ActiveFor)
{
	OnActivated(ActiveFor);

	bIsPowerupActive = true;
	// OnRep is automatically called on clients, but we still need to call it on the server
	OnRep_PowerupActive();

	if (PowerupInterval > 0.f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &APowerupActor::ProcessTick, PowerupInterval, true);
	}
	else
	{
		ProcessTick();
	}
}

void APowerupActor::ProcessTick()
{
	TicksProcessed++;

	OnPowerupTicked();

	if (TicksProcessed >= TotalTicks)
	{
		OnExpired();

		bIsPowerupActive = false;
		// OnRep is automatically called on clients, but we still need to call it on the server
		OnRep_PowerupActive();

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void APowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChanged(bIsPowerupActive);
}

void APowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APowerupActor, bIsPowerupActive);
}
