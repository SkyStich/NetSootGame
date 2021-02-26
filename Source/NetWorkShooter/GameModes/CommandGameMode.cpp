// Fill out your copyright notice in the Description page of Project Settings.

#include "CommandGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NetWorkShooter/PlayerState/CommandPlayerState.h"
#include "NetWorkShooter/Controllers/PlayerControllers/CommandPlayerController.h"
#include "NetWorkShooter/GameStates/BaseGameState.h"
#include "NetWorkShooter/GameStates/CommandGameState.h"
#include "NetWorkShooter/Objects/WeaponObject/MainWeaponObject.h"
#include "NetWorkShooter/Spectators/MoveSpectatorToKiller.h"
#include "NetWorkShooter/PlayerStart/PlayerStartBase.h"
#include "Kismet/KismetMathLibrary.h"

ACommandGameMode::ACommandGameMode()
{
    PlayerControllerClass = ACommandPlayerController::StaticClass();
}

void ACommandGameMode::BeginPlay()
{    
    Super::BeginPlay();

}

void ACommandGameMode::StartGameMatch()
{
    AutoBalanceOfTeams();
    
    Super::StartGameMatch();
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
    FTransform const Transform = LoserController->GetPawn()->GetTransform();

    /** Init spawn parameters */
    FActorSpawnParameters Param;
    Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 

    /** Spawn spectator */
    NewSpectator = GetWorld()->SpawnActor<AMainSpectatorPawn>(AMoveSpectatorToKiller::StaticClass(), Transform, Param);

    if(NewSpectator)
    {
        /** Posses controller in the pawn */
        LoserController->Possess(NewSpectator);

        if(bOurKillerExists)
        {
            FTimerDelegate TimerDel;
            TimerDel.BindUObject(this, &ACommandGameMode::AttachSpectatorToKiller, NewSpectator, DeathInstigator->GetPawn());
            
            FTimerHandle TimerHandle;
            GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 2.f, false);
        }
    }
}

void ACommandGameMode::AttachSpectatorToKiller(AMainSpectatorPawn* Spectator, APawn* Killer)
{
    Spectator->AttachToActor(Killer, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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
                CommandState->ChangeTeamPoints(CommandInstigatorState->GetTeam());
            
                if(CommandState->GetPointByTeam(CommandInstigatorState->GetTeam()) >= MaximumCountOfKillsPerMatch)
                {
                    StopGameMatch("Max kill.");
                }
                return true;
            }
            return false;
        }
        else
        {
            CommandLoserState->IncrementNumberOfDeaths();
            CommandInstigatorState->DecrementNumberOfMurders();

            FString const WeaponName = Cast<ANetWorkShooterCharacter>(InstigatorController->GetPawn())->GetWeaponManager()->GetCurrentWeapon()->GetWeaponName().ToString();
            CommandInstigatorState->NetMulticastOwnerDead(CommandLoserState->GetPlayerName(), CommandInstigatorState->GetPlayerName(), WeaponName);
            return false;
        }
    }
    return Super::UpDateDeathPoints(LoserController, InstigatorController);
}

void ACommandGameMode::AutoBalanceOfTeams()
{
    auto const TeamGameState = Cast<ACommandGameState>(GameState);
    if(TeamGameState)
    {
        int32 const LenghtTeamA = TeamGameState->GetAmountPlayersInTeam(TeamA);
        int32 const LenghtTeamB = TeamGameState->GetAmountPlayersInTeam(TeamB);
        int32 const Difference = abs(LenghtTeamA - LenghtTeamB);
        
        if(Difference > 1)
        {
            for(int32 i = 0; i < Difference / 2; ++i)
            {
                if(LenghtTeamA > LenghtTeamB)
                {
                    TArray<ACommandPlayerState*> TempTeam;
                    TempTeam[UKismetMathLibrary::RandomInteger(TempTeam.Num() - 1)]->SetTeam(TeamB);
                }
                else
                {
                    TArray<ACommandPlayerState*> TempTeam;
                    TeamGameState->GetPlayersInOneTeam(TeamB, TempTeam);
                    TempTeam[UKismetMathLibrary::RandomInteger(TempTeam.Num() - 1)]->SetTeam(TeamA);
                }
            }
        }
    }
}
