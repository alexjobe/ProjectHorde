// Made by Alex Jobe


#include "ShooterAIController.h"
#include "ProjectHorde/ProjectHorde.h"

AShooterAIController::AShooterAIController()
{
	SetGenericTeamId(FGenericTeamId(TEAM_ID_AI));
}

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);
	}
}

ETeamAttitude::Type AShooterAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {

		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
		}
	}

	return ETeamAttitude::Neutral;
}
