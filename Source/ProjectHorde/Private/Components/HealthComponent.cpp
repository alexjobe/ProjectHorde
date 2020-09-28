// Made by Alex Jobe


#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	//Initialize the player's Health
	DefaultHealth = 100.f;
	CurrentHealth = DefaultHealth;
	bIsDead = false;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();

	if (MyOwner && MyOwner->HasAuthority())
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
	}
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsDead || Damage <= 0.f) return;
	SetCurrentHealth(CurrentHealth - Damage);
}

void UHealthComponent::OnRep_CurrentHealth(float OldHealth)
{
	OnHealthUpdate(OldHealth);
}

void UHealthComponent::OnHealthUpdate(float OldHealth)
{
	AActor* MyOwner = GetOwner();

	float HealthChangeAmount = CurrentHealth - OldHealth;
	OnHealthChanged.Broadcast(this, CurrentHealth, HealthChangeAmount, nullptr, nullptr, nullptr);

	if (MyOwner && MyOwner->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Health Changed: %s"), *FString::SanitizeFloat(CurrentHealth));

		if (CurrentHealth <= 0.f)
		{
			bIsDead = true;
		}
	}
}

void UHealthComponent::SetCurrentHealth(float NewHealth)
{
	AActor* MyOwner = GetOwner();
	if (MyOwner && MyOwner->HasAuthority())
	{
		float OldHealth = CurrentHealth;
		CurrentHealth = FMath::Clamp(NewHealth, 0.f, DefaultHealth);
		OnHealthUpdate(OldHealth);
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health.
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}