// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryManForHimselfGameState.h"
#include "Net/UnrealNetwork.h"

AEveryManForHimselfGameState::AEveryManForHimselfGameState()
{
   
}

void AEveryManForHimselfGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AEveryManForHimselfGameState::MatchStart()
{
    if(GetLocalRole() == ROLE_Authority)
    {
        StartGameTimer();
    }
    Super::MatchStart();
}