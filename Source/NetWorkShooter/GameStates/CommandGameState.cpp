// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandGameState.h"
#include "Net/UnrealNetwork.h"

ACommandGameState::ACommandGameState()
{
	Points.SetNum(2);
	Points.Add(0);
	Points.Add(0);
}

void ACommandGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACommandGameState, Points);
}

void ACommandGameState::ChangeTeamPoints(const TEnumAsByte<ETeamList>& Team)
{
	if(Points.IsValidIndex(Team))
		Points[Team]++;
	else
		UE_LOG(LogGameState, Error, TEXT("Is not valid index. Can not add points in team. ACommandGameState(27)"));
}

int32 ACommandGameState::GetPointByTeam(TEnumAsByte<ETeamList> Team)
{
	if(Points.IsValidIndex(Team))
	{
		return Points[Team];
	}
	return 0;
}

TEnumAsByte<ETeamList> ACommandGameState::CheckWinnerTeam()
{
	TEnumAsByte<ETeamList> TempWinner;
            
	GetPointByTeam(ETeamList::TeamA) > GetPointByTeam(ETeamList::TeamB) ? TempWinner = ETeamList::TeamA : TempWinner = ETeamList::TeamB;

	return TempWinner;
}

void ACommandGameState::OnRep_TeamPoints()
{
	OnTeamPointChangedEvent.Broadcast();
}