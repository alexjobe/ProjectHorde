// Made by Alex Jobe


#include "LobbyGameMode.h"

#include "LobbyGameState.h"
#include "MenuInterface.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if (!ensure(LobbyGameState != nullptr)) return;

	LobbyGameState->PlayerJoined();
}

void ALobbyGameMode::BeginStartGameCountdown()
{
	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if (!ensure(LobbyGameState != nullptr)) return;
	LobbyGameState->PrepareToTravel();

	// Set countdown timer to begin game
	GetWorldTimerManager().SetTimer(TimerHandle_Countdown, this, &ALobbyGameMode::StartGame, CountdownDuration);
}

void ALobbyGameMode::StartGame()
{
	IMenuInterface* Interface = Cast<IMenuInterface>(GetGameInstance());
	if (!ensure(Interface != nullptr)) return;

	bUseSeamlessTravel = true;

	Interface->Play();
}
