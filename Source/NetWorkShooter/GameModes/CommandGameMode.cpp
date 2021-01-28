// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NetWorkShooter/Controllers/PlayerControllers/CommandPlayerController.h"
#include "NetWorkShooter/GameStates/BaseGameState.h"
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
    auto const CommandController = Cast<ACommandPlayerController>(SpawnController);
    if(!CommandController) return false;
    
    return Super::PointSelectionConditions(SpawnController, PointToCheck) && PointToCheck->GetTeam() == CommandController->GetTeam();
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


