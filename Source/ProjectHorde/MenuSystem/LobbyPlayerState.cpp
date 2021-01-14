// Made by Alex Jobe


#include "LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"

bool ALobbyPlayerState::GetReadyState() const
{
	return bIsReady;
}

void ALobbyPlayerState::OnRep_ReadyState()
{
	ReadyStateChanged(bIsReady);
}

void ALobbyPlayerState::ToggleReady_Implementation()
{
	bIsReady = !bIsReady;
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
}
