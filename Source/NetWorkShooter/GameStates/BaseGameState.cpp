// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "../NetWorkShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ABaseGameState::ABaseGameState()
{
    /** init base variable */
    MaxTimeBeforeStartOfMatch = 8;
    TimeBeforeStartOfMatch = MaxTimeBeforeStartOfMatch;
    MatchDurationTime = FTimespan(0, 2, 0);
    MatchState = EMatchState::PreStart;
    bReplicates = true;

    /** Set net time update */
    NetUpdateFrequency = 10.f;

    /** Create components */
    ChatComponent = CreateDefaultSubobject<UChatComponent>(TEXT("ChatComponent"));
    ChatComponent->SetIsReplicated(true);
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
            TimeBeforeStartOfMatch -= 1;
            
            if(TimeBeforeStartOfMatch < 0)
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
    
    MatchState = EMatchState::Game;

    MulticastMatchStart();
}

void ABaseGameState::OnRep_MatchState()
{
    OnMatchStateChangedEvent.Broadcast(MatchState);
}

void ABaseGameState::MatchEnd(FString Reason)
{
    MatchState = EMatchState::MatchEnd;
    
    MulticastMatchEnd(Reason);

    TimeBeforeStartOfMatch = MaxTimeBeforeStartOfMatch;
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
    static FTimespan const DecrementTime(0, 0, 1);
    CurrentPlayTime -= DecrementTime;
}

void ABaseGameState::NewPlayerConnected(ABasePlayerState* NewPlayer)
{
    OnNewPlayerConnectedEvent.Broadcast(NewPlayer);
}