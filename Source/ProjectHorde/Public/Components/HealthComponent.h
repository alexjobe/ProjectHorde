// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTHORDE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	// Property replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bIsDead;

	// The owner's default health. This is the value their health starts at when spawned.
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float DefaultHealth;

	// The owner's current health. When reduced to 0, they are dead.
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	// Function to bind to owning actor's OnTakeAnyDamage delegate
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	// RepNotify for changes made to current health
	UFUNCTION()
	void OnRep_CurrentHealth();

	// Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify
	void OnHealthUpdate();

public:

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetDefaultHealth() const { return DefaultHealth; }

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	// Clamps the value between 0 and DefaultHealth and calls OnHealthUpdate. Should only be called on the server.
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float healthValue);
};
