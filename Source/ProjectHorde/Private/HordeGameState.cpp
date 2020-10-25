// Made by Alex Jobe


#include "HordeGameState.h"
#include "Net/UnrealNetwork.h"

void AHordeGameState::OnRep_WaveState(EWaveState OldState)
{
	WaveStateChanged(WaveState, OldState);
}

void AHordeGameState::SetWaveState(EWaveState NewState)
{
	if (HasAuthority())
	{
		EWaveState OldState = WaveState;

		WaveState = NewState;

		// OnRep automatically gets called on clients, but we still need to manually call it on server
		OnRep_WaveState(OldState);
	}
}

void AHordeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHordeGameState, WaveState);
}