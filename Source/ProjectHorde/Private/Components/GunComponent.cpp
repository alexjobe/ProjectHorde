// Made by Alex Jobe


#include "Components/GunComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
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
	BulletSpread = 1.f;
	MaxAmmo = 50;
	ClipSize = 10;
	ReloadTime = 2.f;

	bIsReloading = false;
	AmmoInReserve = FMath::Max(MaxAmmo - ClipSize, 0);
	AmmoInClip = FMath::Max(ClipSize, 0);

	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UGunComponent::BeginPlay()
{
	Super::BeginPlay();

	MeshComp = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	bIsReloading = false;
	AmmoInReserve = FMath::Max(MaxAmmo - ClipSize, 0);
	AmmoInClip = FMath::Max(ClipSize, 0);
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

	if (bIsReloading)
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(ReloadTimer);
		bIsReloading = false;
		OnReloadStateChanged.Broadcast(this, false, true);
	}

	if (AmmoInClip < 1)
	{
		StartReload();
		return;
	}

	AmmoInClip--;
	OnAmmoStateChanged.Broadcast(this, AmmoInClip, AmmoInReserve);

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

void UGunComponent::AddAmmo(int32 AmountToAdd)
{
	int32 ActualAmountToAdd = FMath::Min(MaxAmmo - AmmoInReserve - AmmoInClip, AmountToAdd);
	AmmoInReserve = AmmoInReserve + ActualAmountToAdd;
	OnAmmoStateChanged.Broadcast(this, AmmoInClip, AmmoInReserve);
	if (AmmoInClip < ClipSize)
	{
		StartReload();
	}
}

void UGunComponent::StartReload()
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;

	if (!MyOwner->HasAuthority())
	{
		ServerReload();
		return;
	}

	if (AmmoInReserve > 0 && !bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reloading..."));
		bIsReloading = true;
		OnReloadStateChanged.Broadcast(this, true, false);
		GetOwner()->GetWorldTimerManager().SetTimer(ReloadTimer, this, &UGunComponent::FinishReloading, ReloadTime, false);
	}
}

void UGunComponent::FinishReloading()
{
	int32 ReloadAmount = FMath::Min(ClipSize - AmmoInClip, AmmoInReserve);
	AmmoInClip += ReloadAmount;
	AmmoInReserve -= ReloadAmount;
	OnAmmoStateChanged.Broadcast(this, AmmoInClip, AmmoInReserve);
	
	bIsReloading = false;
	OnReloadStateChanged.Broadcast(this, false, true);
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
	ShotDirection = ApplyBulletSpread(ShotDirection);

	FVector TraceEnd = TraceStart + (ShotDirection * MaxRange);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::White, false, 1.f, 0, 1.f);
	return GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, COLLISION_WEAPON, QueryParams);
}

FVector UGunComponent::ApplyBulletSpread(const FVector& ShotDirection)
{
	float HalfRad = FMath::DegreesToRadians(BulletSpread);
	return FMath::VRandCone(ShotDirection, HalfRad);
}

void UGunComponent::OnRep_HitScanTrace()
{
	// Play cosmetic effects
	PlayFireEffects(HitScanTrace.TraceTo);

	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void UGunComponent::OnRep_IsReloading(bool WasReloading)
{
	OnReloadStateChanged.Broadcast(this, bIsReloading, WasReloading);
}

void UGunComponent::OnRep_AmmoInClip()
{
	OnAmmoStateChanged.Broadcast(this, AmmoInClip, AmmoInReserve);
}

void UGunComponent::OnRep_AmmoInReserve()
{
	OnAmmoStateChanged.Broadcast(this, AmmoInClip, AmmoInReserve);
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

	if (FireMontage)
	{
		ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
		if (OwnerChar)
		{
			OwnerChar->PlayAnimMontage(FireMontage);
		}
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

void UGunComponent::ServerReload_Implementation()
{
	StartReload();
}

bool UGunComponent::ServerReload_Validate()
{
	return true;
}

void UGunComponent::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGunComponent, HitScanTrace);
	DOREPLIFETIME(UGunComponent, bIsReloading);
	DOREPLIFETIME(UGunComponent, AmmoInClip);
	DOREPLIFETIME(UGunComponent, AmmoInReserve);
}