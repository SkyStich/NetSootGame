// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandGameState.h"
#include "Net/UnrealNetwork.h"

ACommandGameState::ACommandGameState()
{
}

void ACommandGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACommandGameState, TeamPoints);
}

void ACommandGameState::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetLocalRole() == ROLE_Authority)
	{
		TeamPoints.Add(ETeamList::TeamA, 0);
		TeamPoints.Add(ETeamList::TeamB, 0);
	}
}

void ACommandGameState::ChangeTeamPoints(const TEnumAsByte<ETeamList>& Team, int32 Value)
{
	int32 const TempPoint = TeamPoints.FindRef(Team) + Value;
	TeamPoints.Add(Team, TempPoint);
}

int32 ACommandGameState::GetPointByTeam(TEnumAsByte<ETeamList> Team)
{
	return TeamPoints.FindRef(Team);
}