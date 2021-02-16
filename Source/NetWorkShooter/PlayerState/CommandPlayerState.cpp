// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandPlayerState.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "NetWorkShooter/GameStates/CommandGameState.h"

ACommandPlayerState::ACommandPlayerState()
{
	Team = TeamNone;
}

void ACommandPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACommandPlayerState, Team);
}

void ACommandPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetLocalRole() == ROLE_Authority)
	{
		FindTeam();
		ForceNetUpdate();
	}
}

void ACommandPlayerState::SetTeam(const TEnumAsByte<ETeamList> NewTeam)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		Team = NewTeam;
		ForceNetUpdate();
	}
}

void ACommandPlayerState::FindTeam()
{
	TArray<int32> AmountPlayerTeams;
	AmountPlayerTeams.SetNum(2);
	AmountPlayerTeams.Add(0);
	AmountPlayerTeams.Add(0);

	for(auto& ByArray : UGameplayStatics::GetGameState(GetWorld())->PlayerArray)
	{
		auto const TempState = Cast<ACommandPlayerState>(ByArray);
		if(TempState->GetTeam() != TeamNone)
		{
			AmountPlayerTeams[TempState->GetTeam()]++;
		}
	}
	AmountPlayerTeams[TeamA] >= AmountPlayerTeams[TeamB] ? SetTeam(TeamB) : SetTeam(TeamA); 
}
