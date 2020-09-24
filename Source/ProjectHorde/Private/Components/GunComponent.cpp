// Made by Alex Jobe


#include "Components/GunComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProjectHorde/ProjectHorde.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	MaxRange = 1000.f;
	Damage = 20.f;
}


void UGunComponent::Shoot()
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;

	AController* OwnerController = GetOwnerController();
	if (!OwnerController) return;

	if (MeshComp)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleGlow, MeshComp, TEXT("Muzzle"));
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, MeshComp, TEXT("Muzzle_01"));
	}

	FHitResult Hit;
	FVector ShotDirection;
	bool bTraceSuccess = ShotTrace(Hit, ShotDirection);

	if (bTraceSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, Hit, OwnerController, MyOwner, DamageType);
		}
	}
}

// Called when the game starts
void UGunComponent::BeginPlay()
{
	Super::BeginPlay();

	MeshComp = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
}

AController* UGunComponent::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return nullptr;
	return OwnerPawn->GetController();
}

bool UGunComponent::ShotTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return false;

	AController* OwnerController = GetOwnerController();
	if (!OwnerController) return false;

	FVector TraceStart;
	FRotator TraceRotation;
	OwnerController->GetPlayerViewPoint(TraceStart, TraceRotation);

	ShotDirection = TraceRotation.Vector();

	FVector TraceEnd = TraceStart + (ShotDirection * MaxRange);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MyOwner);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::White, false, 1.f, 0, 1.f);
	return GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, COLLISION_WEAPON, Params);
}

