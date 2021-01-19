// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "Net/UnrealNetwork.h"

ABaseGameState::ABaseGameState()
{
  
}

void ABaseGameState::BeginPlay()
{
    Super::BeginPlay();

    if(GetLocalRole() == ROLE_Authority)
    {
       // auto const MainGameMode = Cast<ANetWorkShooterGameMode>(AuthorityGameMode);
       // MainGameMode->OnMatchStopEvent.AddDynamic(this, &ABaseGameState::MatchEnd);
       // MainGameMode->MatchStartedEvent.AddDynamic(this, &ABaseGameState::MatchStart);
    }
}

void ABaseGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseGameState, PlayTime);
}

void ABaseGameState::MatchStart()
{
    PlayTime = FTimespan(0, 0, 30);
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
    static FTimespan const DecrementTineFTimespan(0, 0, 1);
    PlayTime -= DecrementTineFTimespan; 

    if(PlayTime == FTimespan(0, 0, 0))
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