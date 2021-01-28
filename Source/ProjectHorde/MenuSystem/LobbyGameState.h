// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHORDE_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	void PlayerJoined();

	UFUNCTION(Server, Reliable)
	void CheckAllPlayersReady();

	UFUNCTION(NetMulticast, Reliable)
	void PrepareToTravel();

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
