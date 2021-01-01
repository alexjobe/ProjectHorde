// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class PROJECTHORDE_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	// Property replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Restart() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	class UCameraComponent* CameraComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	class USpringArmComponent* SpringArmComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	class UGunComponent* GunComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	class UHealthComponent* HealthComp = nullptr;

	// Delay between shots in seconds
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Weapon")
	float FireRate;

	// If true, we are in the process of firing
	bool bIsFiringWeapon;

	// Pawn died
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Gameplay")
	bool bIsDead;

	// Handles fire rate delay between shots
	FTimerHandle FiringTimer;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called for forwards/backward input
	void MoveForward(float Value);

	// Called for side to side input
	void MoveRight(float Value);

	// Rate of 0 to 1, where 1 means 100% of desired turn rate
	void TurnAtRate(float Rate);

	// Rate of 0 to 1, where 1 means 100% of desired turn rate
	void LookUpAtRate(float Rate);

	// Function for beginning weapon fire
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartFire();

	// Function for ending weapon fire. Once this is called, the player can use StartFire again.
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnFire();

	// Reload weapon
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Reload();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnStartReload();

	UFUNCTION()
	void OnHealthChanged(class UHealthComponent* HealthComponent, float Health, float HealthChangeAmount, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnRestart();

public:
	// Base turn rate, in deg/sec
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	// Base look up/down rate, in deg/sec
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
