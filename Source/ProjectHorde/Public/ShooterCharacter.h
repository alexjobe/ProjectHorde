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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	class UCameraComponent* CameraComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	class USpringArmComponent* SpringArmComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	class UGunComponent* GunComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	class UHealthComponent* HealthComp = nullptr;

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

	// Delay between shots in seconds
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Weapon")
	float FireRate;

	// If true, we are in the process of firing
	bool bIsFiringWeapon;

	// Function for beginning weapon fire
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartFire();

	// Function for ending weapon fire. Once this is called, the player can use StartFire again.
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	// Handles fire rate delay between shots
	FTimerHandle FiringTimer;


public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Base turn rate, in deg/sec
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	// Base look up/down rate, in deg/sec
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;
};
