// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryManForHimselfGameState.h"
#include "Net/UnrealNetwork.h"

AEveryManForHimselfGameState::AEveryManForHimselfGameState()
{
    MatchDurationTime = FTimespan(0, 2, 0);
    MatchWithKillLimit = 5;
}

void AEveryManForHimselfGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AEveryManForHimselfGameState, CurrentKillInMatch);
}

void AEveryManForHimselfGameState::UpdateTheKillCounter(AController* LoserController, AController* DeathInstigator, AActor* KillingCauser)
{
    Super::UpdateTheKillCounter(LoserController, DeathInstigator, KillingCauser);
    
    if(GetLocalRole() == ROLE_Authority)
    {
        if(LoserController != DeathInstigator)
        {
            CurrentKillInMatch++;

            if(CurrentKillInMatch >= MatchWithKillLimit)
            {
                OnExcessDeathsEvent.Broadcast();
            }
        }
    }
}