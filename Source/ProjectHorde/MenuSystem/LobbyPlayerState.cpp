// Made by Alex Jobe


#include "LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"

#include "LobbyGameState.h"

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
	if (bIsReady)
	{
		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;

		ALobbyGameState* GameState = World->GetGameState<ALobbyGameState>();
		if (!ensure(GameState != nullptr)) return;

		GameState->CheckAllPlayersReady();
	}
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
}
