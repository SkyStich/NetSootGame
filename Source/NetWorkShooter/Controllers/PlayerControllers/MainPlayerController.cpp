// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "NetWorkShooter/Spectators/MainSpectatorPawn.h"
#include "NetWorkShooter/NetWorkShooterGameMode.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "NetWorkShooter/HUD/BaseHUD/BaseHUD.h"
#include "Net/UnrealNetwork.h"

AMainPlayerController::AMainPlayerController()
{
    bReplicates = true;

    NetUpdateFrequency = 5.f;
}

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay(); 
    
    if(GetLocalRole() == ROLE_Authority)
    {
        Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()))->OnMatchEndedEvent.AddDynamic(this, &AMainPlayerController::MatchEnded);
    }
}

void AMainPlayerController::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AMainPlayerController::SpawnPlayer()
{
    auto const GameMode = Cast<ANetWorkShooterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if(GameMode)
    {
        GameMode->SpawnPlayer(this);
    }
}

void AMainPlayerController::MatchEnded(FString Reason)
{
    DisableInput(this);
}

void AMainPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    if(InputComponent == nullptr)
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
    RespawnDelegateTimer.BindUFunction(this, "PlayerRespawn");
    GetWorld()->GetTimerManager().SetTimer(RespawnTimer, RespawnDelegateTimer, MainGameMode->GetRespawnTime(), false);   
}

void AMainPlayerController::PlayerRespawn()
{
    if(GetLocalRole() == ROLE_Authority)
    {
        auto const TempPawn = GetPawn();
    
        SpawnPlayer();
    
        TempPawn->Destroy();
    }
}

void AMainPlayerController::ToggleTabMenu()
{
    if(GetLocalRole() != ROLE_Authority)
    {
        auto const HUD = GetHUD<ABaseHUD>();
        if(HUD)
        {
            HUD->ShowTabMenu();
        }
    }
}

void AMainPlayerController::HideTabMenu()
{
    if(GetLocalRole() != ROLE_Authority)
    {
        auto const HUD = GetHUD<ABaseHUD>();
        if(HUD)
        {
            HUD->HiddenTabMenu();
        }
    }
}

void AMainPlayerController::OnRep_Pawn()
{
    Super::OnRep_Pawn();
    OnNewPawnEvent.Broadcast(GetPawn());
}