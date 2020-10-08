// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunComponent.generated.h"

// Contains information about a single hit scan weapon line trace
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceTo;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHORDE_API UGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunComponent();

	// Property replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Shoot();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Damage")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Shooting")
	float MaxRange;

	// Bullet spread in degrees
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Shooting", meta = (ClampMin = 0.f))
	float BulletSpread;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Ammo", meta = (ClampMin = 0.f))
	int32 MaxAmmo;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay|Ammo", meta = (ClampMin = 0.f))
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Ammo", meta = (ClampMin = 0.f))
	int32 ClipSize;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay|Ammo", meta = (ClampMin = 0.f))
	int32 NumRoundsInClip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* DefaultImpactEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* FleshImpactEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* MuzzleFlash = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* MuzzleGlow = nullptr;

	USkeletalMeshComponent* MeshComp = nullptr;

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	// Called when the game starts
	virtual void BeginPlay() override;

	AController* GetOwnerController() const;

	bool ShotTrace(FHitResult& Hit, FVector& ShotDirection);

	FVector ApplyBulletSpread(const FVector& ShotDirection);

	void ProcessHit(FHitResult& Hit, FVector& ShotDirection);

	void Reload();

	UFUNCTION()
	void OnRep_HitScanTrace();

	void PlayFireEffects(FVector TracerEndPoint);

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	// Server function for shooting
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShoot();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Damage")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Damage")
	float CritHitMultiplier;
};
