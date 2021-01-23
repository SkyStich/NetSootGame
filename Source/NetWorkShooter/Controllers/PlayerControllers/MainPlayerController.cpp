// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "NetWorkShooter/Spectators/MainSpectatorPawn.h"
#include "NetWorkShooter/NetWorkShooterGameMode.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "NetWorkShooter/HUD/BaseHUD/BaseHUD.h"

AMainPlayerController::AMainPlayerController()
{
    bReplicates = true;
}

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();
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

    InputComponent->BindAction("TabMenu", IE_Pressed, this, &AMainPlayerController::ToggleTabMenu);
    InputComponent->BindAction("TabMenu", IE_Released, this, &AMainPlayerController::HideTabMenu);
}

void AMainPlayerController::LaunchRespawnTimer(float const TimeToRespawn, class ANetWorkShooterGameMode* MainGameMode)
{
    FTimerDelegate RespawnDelegateTimer;
    RespawnDelegateTimer.BindUFunction(this, "ServerPlayerRespawn");
    GetWorld()->GetTimerManager().SetTimer(RespawnTimer, RespawnDelegateTimer, MainGameMode->GetRespawnTime(), false);   
}

void AMainPlayerController::ServerPlayerRespawn_Implementation()
{
    auto const MainGameMode = Cast<ANetWorkShooterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    
    auto const TempPawn = GetPawn();
    MainGameMode->SpawnPlayer(this);
    TempPawn->Destroy();
}

void AMainPlayerController::ToggleTabMenu()
{
    if(GetLocalRole() != ROLE_Authority)
    {
        GetHUD<ABaseHUD>()->ShowTabMenu();
    }
}

void AMainPlayerController::HideTabMenu()
{
    if(GetLocalRole() != ROLE_Authority)
    {
        GetHUD<ABaseHUD>()->HiddenTabMenu();
    }
}