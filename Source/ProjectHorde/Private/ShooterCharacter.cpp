// Made by Alex Jobe


#include "ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/GunComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bIsFiringWeapon = false;
	FireRate = 0.5f;

	// Create camera spring arm
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	// Create follow camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// Create gun component
	GunComp = CreateDefaultSubobject<UGunComponent>(TEXT("GunComp"));

	// Create health component
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	bIsDead = false;

	GetMovementComponent()->NavAgentProps.bCanCrouch = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComp)
	{
		HealthComp->OnHealthChanged.AddDynamic(this, &AShooterCharacter::OnHealthChanged);
	}
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Handle firing projectiles
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooterCharacter::StartFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShooterCharacter::Reload);
}

void AShooterCharacter::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorForwardVector() * Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorRightVector() * Value);
	}
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		GetWorld()->GetTimerManager().SetTimer(FiringTimer, this, &AShooterCharacter::StopFire, FireRate, false);
		if (GunComp) GunComp->Shoot();
	}
}

void AShooterCharacter::StopFire()
{
	bIsFiringWeapon = false;
}

void AShooterCharacter::Reload()
{
	if (GunComp) GunComp->StartReload();
}

void AShooterCharacter::OnHealthChanged(UHealthComponent* HealthComponent, float Health, float HealthChangeAmount, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.f && !bIsDead)
	{
		// Time to die
		bIsDead = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.f);
	}
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterCharacter, bIsDead);
}