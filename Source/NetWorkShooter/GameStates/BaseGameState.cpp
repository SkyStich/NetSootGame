// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "../NetWorkShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ABaseGameState::ABaseGameState()
{
    MatchDurationTime = FTimespan(0, 2, 0);
}

void ABaseGameState::BeginPlay()
{
    Super::BeginPlay();

    if(GetLocalRole() == ROLE_Authority)
    {
        auto const MainGameMode = Cast<ANetWorkShooterGameMode>(AuthorityGameMode);
        MainGameMode->OnMatchStopEvent.AddDynamic(this, &ABaseGameState::MatchEnd);
        MainGameMode->MatchStartedEvent.AddDynamic(this, &ABaseGameState::MatchStart);
    }
}

void ABaseGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseGameState, CurrentPlayTime);
}

void ABaseGameState::MatchStart()
{
    CurrentPlayTime = MatchDurationTime;
    MulticastMatchStart();
}

void ABaseGameState::MatchEnd(FString Reason)
{
    MulticastMatchEnd(Reason);
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

