// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "../NetWorkShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ABaseGameState::ABaseGameState()
{
    TimeBeforeStartOfMatch = 5;
    MatchDurationTime = FTimespan(0, 2, 0);
    MatchState = EMatchState::PreStart;
}

void ABaseGameState::BeginPlay()
{
    Super::BeginPlay();

    if(GetLocalRole() == ROLE_Authority)
    {
        auto const MainGameMode = Cast<ANetWorkShooterGameMode>(AuthorityGameMode);
        MainGameMode->OnMatchStopEvent.AddDynamic(this, &ABaseGameState::MatchEnd);

        GetWorld()->GetTimerManager().SetTimer(GameStartTimer, this, &ABaseGameState::GameStartTime, 1.f, true);
    }
}

void ABaseGameState::GameStartTime()
{
    if(GetLocalRole() == ROLE_Authority)
    {
        if(MatchState == EMatchState::PreStart)
        {
            TimeBeforeStartOfMatch--;
            if(TimeBeforeStartOfMatch <= 0)
            {                
                Cast<ANetWorkShooterGameMode>(AuthorityGameMode)->StartGameMatch();
                MatchStart();
                GetWorld()->GetTimerManager().ClearTimer(GameStartTimer);
            }
        }
    }
}

void ABaseGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseGameState, CurrentPlayTime);
    DOREPLIFETIME(ABaseGameState, MatchState);
    DOREPLIFETIME(ABaseGameState, TimeBeforeStartOfMatch);
}

void ABaseGameState::MatchStart()
{
    CurrentPlayTime = MatchDurationTime;

    MulticastMatchStart();
    
    MatchState = EMatchState::Game;
}

void ABaseGameState::OnRep_MatchState()
{
    OnMatchStateChangedEvent.Broadcast(MatchState);
}

void ABaseGameState::MatchEnd(FString Reason)
{
    MulticastMatchEnd(Reason);

    TimeBeforeStartOfMatch = 5;
    MatchState = EMatchState::MatchEnd;
}

void ABaseGameState::MulticastMatchEnd_Implementation(const FString& Reason)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, Reason);
    OnMatchEndedEvent.Broadcast(Reason);
}

void ABaseGameState::MulticastMatchStart_Implementation()
{
    OnMatchStartedEvent.Broadcast();
}

void ABaseGameState::IncrementPlayTime()
{
    static FTimespan const DecrementTime(0, 0, 1);
    CurrentPlayTime -= DecrementTime;
}

