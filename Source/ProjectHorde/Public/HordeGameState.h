// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HordeGameState.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart,

	WaveInProgress,

	// No longer spawning new bots, waiting for players to kill remaining bots
	WaitingToComplete,

	WaveComplete,

	GameOver
};

UCLASS()
class PROJECTHORDE_API AHordeGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// Property replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
	EWaveState WaveState;

	UFUNCTION()
	// ReplicatedUsing lets us pass in whatever the previous value was (i.e. OldState)
	void OnRep_WaveState(EWaveState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
	void WaveStateChanged(EWaveState NewState, EWaveState OldState);

public:
	void SetWaveState(EWaveState NewState);
};
