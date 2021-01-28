// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHORDE_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	void PostLogin(APlayerController* NewPlayer) override;

	void BeginStartGameCountdown();

private:

	void StartGame();

	float CountdownDuration = 10.f;

	FTimerHandle TimerHandle_Countdown;
};
