// Made by Alex Jobe


#include "LobbyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "LobbyGameMode.h"
#include "LobbyPlayerState.h"
#include "MenuInterface.h"

void ALobbyGameState::PlayerJoined()
{
	IMenuInterface* MenuInterface = Cast<IMenuInterface>(GetGameInstance());
	if (!ensure(MenuInterface != nullptr)) return;

	TArray<ALobbyPlayerState*> LobbyPlayerArray;

	for (auto Player : PlayerArray)
	{
		LobbyPlayerArray.Push(Cast<ALobbyPlayerState>(Player));
	}

	MenuInterface->UpdateLobbyList(LobbyPlayerArray);
}

void ALobbyGameState::CheckAllPlayersReady_Implementation()
{
	if (PlayerArray.Num() < 2) return;

	for (auto Player : PlayerArray)
	{
		ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(Player);
		if (LobbyPlayerState->GetReadyState() != true) return;
	}

	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (!ensure(GameMode != nullptr)) return;

	GameMode->BeginStartGameCountdown();
}

void ALobbyGameState::PrepareToTravel_Implementation()
{
	IMenuInterface* MenuInterface = Cast<IMenuInterface>(GetGameInstance());
	if (!ensure(MenuInterface != nullptr)) return;
	MenuInterface->BeginCountdown();
}

void ALobbyGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	IMenuInterface* MenuInterface = Cast<IMenuInterface>(GetGameInstance());
	if (!ensure(MenuInterface != nullptr)) return;
	MenuInterface->TeardownLobbyMenu();
}
