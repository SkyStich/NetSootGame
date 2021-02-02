// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryManForHimselfGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AEveryManForHimselfGameState::AEveryManForHimselfGameState()
{
    MatchDurationTime = FTimespan(0, 2, 0);
}

void AEveryManForHimselfGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AEveryManForHimselfGameState, CurrentKillInMatch);
}