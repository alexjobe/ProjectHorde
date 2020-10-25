// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HordeGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, DeadActor, AActor*, KillerActor, AController*, KillerController);

enum class EWaveState : uint8;

UCLASS()
class PROJECTHORDE_API AHordeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHordeGameMode();

protected:
	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWaveStart;

	int32 WaveCount;

	// Number to multiply WaveCount by on each successive wave
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	int32 WaveCountMultiplier;

	// Bots left to spawn in current wave
	int32 NumBotsLeftToSpawn;

	// Seconds between each spawn
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float SpawnRate;

	// Seconds between each wave
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;

	// Hook for BP to spawn a single bot
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	// Start spawning bots
	void StartWave();

	// Stop spawning bots
	void EndWave();

	// Start timer for next StartWave
	void PrepareForNextWave();

	void CheckWaveState();

	void CheckAnyPlayerAlive();

	void GameOver();

	void SetWaveState(EWaveState NewState);

	void RespawnDeadPlayers();

public:

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnActorKilled OnActorKilled;
};
