// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

void ACommandPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACommandPlayerState, Team);
}

void ACommandPlayerState::BeginPlay()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		UKismetMathLibrary::RandomBool() ? Team = ETeamList::TeamA : Team = ETeamList::TeamB;
	}
}
