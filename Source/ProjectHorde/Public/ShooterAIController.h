// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTHORDE_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AShooterAIController();

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	class UBehaviorTree* AIBehavior = nullptr;

	virtual void BeginPlay() override;
};
