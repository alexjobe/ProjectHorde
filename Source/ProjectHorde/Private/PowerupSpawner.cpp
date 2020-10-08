// Made by Alex Jobe


#include "PowerupSpawner.h"
#include "Components/SphereComponent.h"
#include "PowerupActor.h"

// Sets default values
APowerupSpawner::APowerupSpawner()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(50.f);
	RootComponent = SphereComp;

	CooldownDuration = 20.f;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void APowerupSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Respawn();
	}
}

void APowerupSpawner::Respawn()
{
	if (!PowerUpClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerUpClass is null in %s. Please update the Blueprint."), *GetName());
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PowerUpInstance = GetWorld()->SpawnActor<APowerupActor>(PowerUpClass, GetTransform(), SpawnParams);
}

void APowerupSpawner::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (HasAuthority() && PowerUpInstance)
	{
		PowerUpInstance->ActivatePowerup(OtherActor);
		PowerUpInstance = nullptr;

		// Set timer to respawn
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &APowerupSpawner::Respawn, CooldownDuration);
	}
}

