// Made by Alex Jobe


#include "LobbyGameMode.h"

#include "LobbyGameState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if (!ensure(LobbyGameState != nullptr)) return;

	LobbyGameState->PlayerJoined();
}

void ALobbyGameMode::StartGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	bUseSeamlessTravel = true;

	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if (!ensure(LobbyGameState != nullptr)) return;
	LobbyGameState->PrepareToTravel();

	UE_LOG(LogTemp, Warning, TEXT("Traveling..."));

	World->ServerTravel("/Game/Maps/TestMap?listen");
}
