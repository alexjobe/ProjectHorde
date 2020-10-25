// Made by Alex Jobe


#include "HordePlayerController.h"
#include "ProjectHorde/ProjectHorde.h"

AHordePlayerController::AHordePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	TeamId = FGenericTeamId(TEAM_ID_PLAYERS);
}

FGenericTeamId AHordePlayerController::GetGenericTeamId() const
{
	return TeamId;
}