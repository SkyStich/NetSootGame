// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "NetWorkShooter/Spectators/MainSpectatorPawn.h"
#include "NetWorkShooter/NetWorkShooterGameMode.h"
#include "NetWorkShooter/NetWorkShooterGameMode.h"
#include "Kismet/GameplayStatics.h"

AMainPlayerController::AMainPlayerController()
{
    bReplicates = true;
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

void AMainPlayerController::LauchRespawnTimer(float const TimeBeforeRespawn)
{
    if(GetLocalRole() == ROLE_Authority)
    {
        
    }
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