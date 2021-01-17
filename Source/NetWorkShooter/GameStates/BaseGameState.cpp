// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "Net/UnrealNetwork.h"

void ABaseGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseGameState, PlayTime);
}

void ABaseGameState::IncrementPlayTime()
{
    FTimespan const TempSpan(0, 0, 1);
    PlayTime += TempSpan;
}

void ABaseGameState::StartGameTimer()
{
    GetWorld()->GetTimerManager().SetTimer(TimeTickHandle, this, &ABaseGameState::IncrementPlayTime, 1, true);
}