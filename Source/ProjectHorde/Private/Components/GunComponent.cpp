// Made by Alex Jobe


#include "Components/GunComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProjectHorde/ProjectHorde.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	MaxRange = 1000.f;
	Damage = 20.f;
	SetIsReplicatedByDefault(true);
}

void UGunComponent::Shoot()
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;

	AController* MyController = MyOwner->GetInstigatorController();
	if (!MyController) return;

	if (!MyOwner->HasAuthority())
	{
		ServerShoot();
	}

	PlayFireEffects(HitScanTrace.TraceTo);

	FHitResult Hit;
	FVector ShotDirection;
	bool bTraceSuccess = ShotTrace(Hit, ShotDirection);

	if (bTraceSuccess)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, Hit, MyController, MyOwner, DamageType);
		}

		if (MyOwner->HasAuthority())
		{
			HitScanTrace.TraceTo = Hit.Location;
			HitScanTrace.SurfaceType = EPhysicalSurface::SurfaceType_Default;
		}

		PlayImpactEffects(HitScanTrace.SurfaceType, Hit.Location);
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

	AController* MyController = MyOwner->GetInstigatorController();
	if (!MyController) return false;

	FVector TraceStart;
	FRotator TraceRotation;
	MyController->GetPlayerViewPoint(TraceStart, TraceRotation);

	ShotDirection = TraceRotation.Vector();

	FVector TraceEnd = TraceStart + (ShotDirection * MaxRange);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MyOwner);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::White, false, 1.f, 0, 1.f);
	return GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, COLLISION_WEAPON, Params);
}

void UGunComponent::OnRep_HitScanTrace()
{
	// Play cosmetic effects
	PlayFireEffects(HitScanTrace.TraceTo);

	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void UGunComponent::PlayFireEffects(FVector TracerEndPoint)
{
	if (!MeshComp) return;

	if (MuzzleGlow)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleGlow, MeshComp, TEXT("Muzzle"));
	}

	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, MeshComp, TEXT("Muzzle_01"));
	}
}

void UGunComponent::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	if (MeshComp && ImpactEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(TEXT("Muzzle"));

		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, ImpactPoint, ShotDirection.Rotation());
	}
}

void UGunComponent::ServerShoot_Implementation()
{
	Shoot();
}

bool UGunComponent::ServerShoot_Validate()
{
	return true;
}

void UGunComponent::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UGunComponent, HitScanTrace, COND_SkipOwner);
}