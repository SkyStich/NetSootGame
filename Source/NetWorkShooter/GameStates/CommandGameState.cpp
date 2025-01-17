// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandGameState.h"
#include "Net/UnrealNetwork.h"
#include "NetWorkShooter/PlayerState/CommandPlayerState.h"

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

void ACommandGameState::GetPlayersInOneTeam(TEnumAsByte<ETeamList> Team, TArray<ACommandPlayerState*>& PlayersInTeam)
{
	for(auto& ByArray : PlayerArray)
	{
		auto const LocalState = Cast<ACommandPlayerState>(ByArray);
		if(LocalState)
		{
			if(LocalState->GetTeam() == Team)
			{
				PlayersInTeam.Add(LocalState);
			}
		}
		else
		{
			UE_LOG(LogGameState, Error, TEXT("Cast can not be create ACommandGameMode(57"));
		}
	}
}

TArray<ACommandPlayerState*> ACommandGameState::GetPlayersInOneTeam(TEnumAsByte<ETeamList> Team)
{
	TArray<ACommandPlayerState*> PlayersInTeam;
	for(auto& ByArray : PlayerArray)
	{
		auto const LocalState = Cast<ACommandPlayerState>(ByArray);
		if(LocalState)
		{
			if(LocalState->GetTeam() == Team)
			{
				PlayersInTeam.Add(LocalState);
			}
		}
		else
		{
			UE_LOG(LogGameState, Error, TEXT("Cast can not be create ACommandGameMode(57"));
		}
	}
	return PlayersInTeam;
}

int32 ACommandGameState::GetAmountPlayersInTeam(TEnumAsByte<ETeamList> TeamForFind)
{
	int32 TempAmount = 0;
	for(auto& ByArray : PlayerArray)
	{
		if(Cast<ACommandPlayerState>(ByArray)->GetTeam() == TeamForFind)
		{
			TempAmount++;
		}
	}
	return TempAmount;
}