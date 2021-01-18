// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "Net/UnrealNetwork.h"

ABaseGameState::ABaseGameState()
{
    PlayTime = FTimespan(0, 0, 30);
}

void ABaseGameState::BeginPlay()
{
    Super::BeginPlay();

    if(GetLocalRole() == ROLE_Authority)
    {
        MainGameMode = Cast<ANetWorkShooterGameMode>(AuthorityGameMode);
        MainGameMode->OnMatchStopEvent.AddDynamic(this, &ABaseGameState::MatchEnd);
        MainGameMode->MatchStartedEvent.AddDynamic(this, &ABaseGameState::MatchStart);
    }
}

void ABaseGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseGameState, PlayTime);
}

void ABaseGameState::MatchStart()
{
    StartGameTimer();
    MulticastMatchStart();
}

void ABaseGameState::MatchEnd(FString Reason)
{
    GetWorld()->GetTimerManager().ClearTimer(TimeTickHandle);
    MulticastMatchEnd(Reason);
}

void ABaseGameState::MulticastMatchEnd_Implementation(const FString& Reason)
{
    OnMatchEndedEvent.Broadcast(Reason);
}

void ABaseGameState::MulticastMatchStart_Implementation()
{
    OnMatchStartedEvent.Broadcast();
}

void ABaseGameState::IncrementPlayTime()
{
    PlayTime -= FTimespan(0, 0, 1);

    if(PlayTime.IsZero())
    {
        OnMatchTimeIsOverEvent.Broadcast();
    }
}

void ABaseGameState::StartGameTimer()
{
    if(GetLocalRole() == ROLE_Authority)
    {
        GetWorld()->GetTimerManager().SetTimer(TimeTickHandle, this, &ABaseGameState::IncrementPlayTime, 1, true);
    }
}