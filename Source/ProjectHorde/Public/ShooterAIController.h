// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHORDE_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	class UBehaviorTree* AIBehavior = nullptr;

	virtual void BeginPlay() override;
};
