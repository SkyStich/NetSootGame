// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NetWorkShooter/PlayerState/CommandPlayerState.h"
#include "NetWorkShooter/Controllers/PlayerControllers/CommandPlayerController.h"
#include "NetWorkShooter/GameStates/BaseGameState.h"
#include "NetWorkShooter/GameStates/CommandGameState.h"
#include "NetWorkShooter/Spectators/MoveSpectatorToKiller.h"
#include "NetWorkShooter/PlayerStart/PlayerStartBase.h"

ACommandGameMode::ACommandGameMode()
{
    PlayerControllerClass = ACommandPlayerController::StaticClass();
}

void ACommandGameMode::BeginPlay()
{
    Super::BeginPlay();
}

bool ACommandGameMode::PointSelectionConditions(AController* SpawnController, APlayerStartBase* PointToCheck)
{
    auto const CommandPlayerState = Cast<ACommandPlayerState>(SpawnController->PlayerState);
    if(!CommandPlayerState) return false;
    
    return Super::PointSelectionConditions(SpawnController, PointToCheck) && CommandPlayerState->GetTeam() == PointToCheck->GetTeam();
}

void ACommandGameMode::SpawnSpectator(AController* LoserController, AController* DeathInstigator, AMainSpectatorPawn*& NewSpectator)
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

bool ACommandGameMode::UpDateDeathPoints(AController* LoserController, AController* InstigatorController)
{    
    if(InstigatorController)
    {
        auto const CommandInstigatorState = InstigatorController->GetPlayerState<ACommandPlayerState>();
        auto const CommandLoserState = LoserController->GetPlayerState<ACommandPlayerState>();
        
        if(CommandInstigatorState->GetTeam() != CommandLoserState->GetTeam())
        {
            if(Super::UpDateDeathPoints(LoserController, InstigatorController))
            {
                auto const CommandState = GetGameState<ACommandGameState>();
                CommandState->ChangeTeamPoints(CommandInstigatorState->GetTeam(), 1);
            
                if(CommandState->GetPointByTeam(CommandInstigatorState->GetTeam()) >= MaximumCountOfKillsPerMatch)
                {
                    StopGameMatch("Max kill.");
                    CommandState->GameWinnerFound(CommandInstigatorState->GetTeam());
                }
                return true;
            }
        }
        else
        {
            CommandLoserState->IncrementNumberOfDeaths();
            CommandInstigatorState->DecrementNumberOfMurders();
            return false;
        }
    }
    return Super::UpDateDeathPoints(LoserController, InstigatorController);
}


