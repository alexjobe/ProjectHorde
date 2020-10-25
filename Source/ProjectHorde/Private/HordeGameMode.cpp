// Made by Alex Jobe


#include "HordeGameMode.h"
#include "Components/HealthComponent.h"
#include "EngineUtils.h"
#include "HordeGameState.h"

AHordeGameMode::AHordeGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f;

	WaveCount = 1;
	WaveCountMultiplier = 2;
	SpawnRate = 1.f;
	TimeBetweenWaves = 2.f;
}

void AHordeGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
	CheckAnyPlayerAlive();
}

void AHordeGameMode::StartWave()
{
	NumBotsLeftToSpawn = WaveCount * WaveCountMultiplier;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &AHordeGameMode::SpawnBotTimerElapsed, SpawnRate, true, 0.f);

	SetWaveState(EWaveState::WaveInProgress);

	WaveCount++;
}

void AHordeGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	SetWaveState(EWaveState::WaitingToComplete);
}

void AHordeGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AHordeGameMode::StartWave, TimeBetweenWaves, false);

	RespawnDeadPlayers();

	SetWaveState(EWaveState::WaitingToStart);
}

void AHordeGameMode::CheckWaveState()
{
	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if (NumBotsLeftToSpawn > 0 || bIsPreparingForWave) return;

	bool bIsAnyBotAlive = false;

	// Check if any bots are still alive
	for (TActorIterator<APawn> PawnIterator(GetWorld()); PawnIterator; ++PawnIterator)
	{
		APawn* TestPawn = *PawnIterator;

		if (!TestPawn || TestPawn->IsPlayerControlled()) continue;

		UHealthComponent* HealthComp = Cast<UHealthComponent>(TestPawn->GetComponentByClass(UHealthComponent::StaticClass()));

		if (HealthComp && HealthComp->GetCurrentHealth() > 0.f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	// If all bots are dead, prepare for next wave
	if (!bIsAnyBotAlive)
	{
		SetWaveState(EWaveState::WaveComplete);
		PrepareForNextWave();
	};
}

void AHordeGameMode::CheckAnyPlayerAlive()
{
	for (TActorIterator<APlayerController> PCIterator(GetWorld()); PCIterator; ++PCIterator)
	{
		APlayerController* PC = *PCIterator;

		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			UHealthComponent* HealthComp = Cast<UHealthComponent>(MyPawn->GetComponentByClass(UHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetCurrentHealth() > 0.f)
			{
				// A player is still alive
				return;
			}
		}
	}

	// No player alive
	GameOver();
}

void AHordeGameMode::GameOver()
{
	EndWave();

	SetWaveState(EWaveState::GameOver);
}

void AHordeGameMode::SetWaveState(EWaveState NewState)
{
	AHordeGameState* GS = GetGameState<AHordeGameState>();

	if (ensureAlways(GS))
	{
		GS->SetWaveState(NewState);
	}
}

void AHordeGameMode::RespawnDeadPlayers()
{
	for (TActorIterator<APlayerController> PCIterator(GetWorld()); PCIterator; ++PCIterator)
	{
		APlayerController* PC = *PCIterator;
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}

void AHordeGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void AHordeGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NumBotsLeftToSpawn--;

	if (NumBotsLeftToSpawn <= 0)
	{
		EndWave();
	}
}