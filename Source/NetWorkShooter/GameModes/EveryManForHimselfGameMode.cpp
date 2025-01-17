// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryManForHimselfGameMode.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../Spectators/MoveSpectatorToKiller.h"
#include "../PlayerState/BasePlayerState.h"
#include "NetWorkShooter/GameStates/EveryManForHimselfGameState.h"

AEveryManForHimselfGameMode::AEveryManForHimselfGameMode()
{
    PlayerStateClass = ABasePlayerState::StaticClass();
    GameStateClass = AEveryManForHimselfGameState::StaticClass();

    MatchWithKillLimit = 5;
}

void AEveryManForHimselfGameMode::BeginPlay()
{
    Super::BeginPlay();
    
}

bool AEveryManForHimselfGameMode::UpDateDeathPoints(AController* LoserController, AController* InstigatorController)
{
    if(Super::UpDateDeathPoints(LoserController, InstigatorController))
    {
        auto const TempGameState = Cast<AEveryManForHimselfGameState>(GameState);
        TempGameState->IncrementCurrentKillInMatch();
  
        if(TempGameState->GetCurrentKillInMatch() >= MatchWithKillLimit)
        {
            StopGameMatch("MaxKill");
        }
        return true;
    }
    return false;
}

void AEveryManForHimselfGameMode::SpawnSpectator(AController* LoserController, AController* DeathInstigator , AMainSpectatorPawn* & NewSpectator)
{
    Super::SpawnSpectator(LoserController, DeathInstigator, NewSpectator);

    /** If the controller of another player is responsible for the death of the player and we did not die from out own hand
    (from the explosion of our own grenade) */
    bool const bOurKillerExists = DeathInstigator && LoserController != DeathInstigator;

    /** If we have a killler we follow him, otherwise we follow the place of death */
    FTransform const Transform = bOurKillerExists ? DeathInstigator->GetPawn()->GetTransform() : LoserController->GetPawn()->GetTransform();

    /** Init spawn parameters */
    FActorSpawnParameters Param;
    Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 

    /** Spawn spectator */
    NewSpectator = GetWorld()->SpawnActor<AMainSpectatorPawn>(AMoveSpectatorToKiller::StaticClass(), Transform, Param);
    
    if(NewSpectator)
    {
        /** Posses controller in the pawn */
        LoserController->Possess(NewSpectator);

        /** If we have a killler we follow him */
        if(bOurKillerExists)
        {
            NewSpectator->AttachToActor(DeathInstigator->GetPawn(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        }
    }     
}