// Made by Alex Jobe


#include "Components/GunComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ProjectHorde/ProjectHorde.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	MaxRange = 5000.f;
	BaseDamage = 20.f;
	CritHitMultiplier = 3.f;
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UGunComponent::BeginPlay()
{
	Super::BeginPlay();

	MeshComp = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
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
		return;
	}

	PlayFireEffects(HitScanTrace.TraceTo);

	FHitResult Hit;
	FVector ShotDirection;
	bool bTraceSuccess = ShotTrace(Hit, ShotDirection);

	if (bTraceSuccess)
	{
		ProcessHit(Hit, ShotDirection);
	}
}

void UGunComponent::ProcessHit(FHitResult& Hit, FVector& ShotDirection)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;

	AController* MyController = MyOwner->GetInstigatorController();
	if (!MyController) return;

	AActor* HitActor = Hit.GetActor();
	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

	float ActualDamage = BaseDamage;

	if (SurfaceType == SURFACE_FLESH_CRITICAL)
	{
		ActualDamage = BaseDamage * CritHitMultiplier;
	}

	if (HitActor)
	{
		UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyController, MyOwner, DamageType);
	}

	HitScanTrace.TraceTo = Hit.Location;
	HitScanTrace.SurfaceType = SurfaceType;

	PlayImpactEffects(SurfaceType, Hit.Location);
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

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::White, false, 1.f, 0, 1.f);
	return GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, COLLISION_WEAPON, QueryParams);
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
	UParticleSystem* SelectedImpactEffect = nullptr;

	switch (SurfaceType)
	{
	case SURFACE_FLESH_DEFAULT:
	case SURFACE_FLESH_CRITICAL:
		SelectedImpactEffect = FleshImpactEffect;
		break;
	default:
		SelectedImpactEffect = DefaultImpactEffect;
		break;
	}

	if (MeshComp && SelectedImpactEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(TEXT("Muzzle"));

		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedImpactEffect, ImpactPoint, ShotDirection.Rotation());
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

	DOREPLIFETIME(UGunComponent, HitScanTrace);
}