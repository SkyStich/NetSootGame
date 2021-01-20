// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "NetWorkShooter/Spectators/MainSpectatorPawn.h"
#include "NetWorkShooter/NetWorkShooterGameMode.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"

AMainPlayerController::AMainPlayerController()
{
    bReplicates = true;
}

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();
   // if(GetLocalRole() == ROLE_Authority)
    //{
       // OnNewPawn.AddLambda([&](APawn* NewPawn)-> void
        //{
          //  if(NewPawn->GetClass() == AMainSpectatorPawn::StaticClass())
            //{
                
            //}
       // });
  //  }
}

void AMainPlayerController::SpawnPlayer()
{
    auto const GameMode = Cast<ANetWorkShooterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if(GameMode)
    {
        GameMode->SpawnPlayer(this);
    }
}

void AMainPlayerController::SetupInputComponent()
{
    if(InputComponent == NULL)
    {
        InputComponent = NewObject<UInputComponent>(this, TEXT("InputComponent"));
        InputComponent->RegisterComponent();
    }

    InputComponent->BindAction("PlayerRespawn", IE_Released, this, &AMainPlayerController::RespawnKeyReleased);
    InputComponent->BindAction("TabMenu", IE_Pressed, this, &AMainPlayerController::ToggleTabMenu);
    InputComponent->BindAction("TabMenu", IE_Released, this, &AMainPlayerController::HideTabMenu);
}

void AMainPlayerController::LaunchRespawnTimer(float const TimeBeforeRespawn)
{
    if(GetLocalRole() == ROLE_Authority)
    {
//        GetWorld()->GetTimerManager().SetTimer()
    }
}

void AMainPlayerController::FinishCooldownTimer()
{
    
}

void AMainPlayerController::ServerPlayerRespawn_Implementation()
{
    auto const GameMode = Cast<ANetWorkShooterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if(GameMode)
    {
        auto TempPawn = GetPawn();
        GameMode->SpawnPlayer(this);
        TempPawn->Destroy();
    }
}

void AMainPlayerController::RespawnKeyReleased()
{
    if(Cast<AMainSpectatorPawn>(GetPawn()))
    {
        ServerPlayerRespawn();
    }
}