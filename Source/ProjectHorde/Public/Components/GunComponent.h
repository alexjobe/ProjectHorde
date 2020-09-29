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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float MaxRange;

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

	void ProcessHit(FHitResult& Hit, FVector& ShotDirection);

	UFUNCTION()
	void OnRep_HitScanTrace();

	void PlayFireEffects(FVector TracerEndPoint);

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	// Server function for shooting
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShoot();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float CritHitMultiplier;
};
