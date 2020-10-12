// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReloadStateChangedSignature, UGunComponent*, GunComp, bool, bIsReloading, bool, bWasReloading);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAmmoStateChangedSignature, UGunComponent*, GunComp, int32, AmmoInClip, int32, AmmoInReserve);

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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Damage")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Shooting")
	float MaxRange;

	// Bullet spread in degrees
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Shooting", meta = (ClampMin = 0.f))
	float BulletSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Ammo", meta = (ClampMin = 0.f))
	int32 MaxAmmo;

	UPROPERTY(ReplicatedUsing = OnRep_AmmoInReserve, VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay|Ammo", meta = (ClampMin = 0.f))
	int32 AmmoInReserve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Ammo", meta = (ClampMin = 0.f))
	int32 ClipSize;

	UPROPERTY(ReplicatedUsing = OnRep_AmmoInClip, VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay|Ammo", meta = (ClampMin = 0.f))
	int32 AmmoInClip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Ammo")
	float ReloadTime;

	// If true, we are in the process of reloading
	UPROPERTY(ReplicatedUsing = OnRep_IsReloading, BlueprintReadOnly, Category = "Gameplay|Ammo")
	bool bIsReloading;

	// Handles reload delay
	FTimerHandle ReloadTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* DefaultImpactEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* FleshImpactEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* MuzzleFlash = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* MuzzleGlow = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UAnimMontage* FireMontage = nullptr;

	USkeletalMeshComponent* MeshComp = nullptr;

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	// Called when the game starts
	virtual void BeginPlay() override;

	AController* GetOwnerController() const;

	bool ShotTrace(FHitResult& Hit, FVector& ShotDirection);

	FVector ApplyBulletSpread(const FVector& ShotDirection);

	void ProcessHit(FHitResult& Hit, FVector& ShotDirection);

	void FinishReloading();

	void PlayFireEffects(FVector TracerEndPoint);

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	UFUNCTION()
	void OnRep_HitScanTrace();

	UFUNCTION()
	void OnRep_IsReloading(bool WasReloading);

	UFUNCTION()
	void OnRep_AmmoInClip();

	UFUNCTION()
	void OnRep_AmmoInReserve();

	// Server function for shooting
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShoot();

	// Server function for reloading
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FReloadStateChangedSignature OnReloadStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FAmmoStateChangedSignature OnAmmoStateChanged;

public:

	void Shoot();

	UFUNCTION(BlueprintCallable, Category = "Gameplay|Ammo")
	void StartReload();

	UFUNCTION(BlueprintCallable, Category = "Gameplay|Ammo")
	void AddAmmo(int32 AmountToAdd);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Damage")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay|Damage")
	float CritHitMultiplier;
};
