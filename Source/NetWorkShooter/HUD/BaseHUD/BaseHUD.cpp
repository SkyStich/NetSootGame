// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"
#include "NetWorkShooter/Spectators/MainSpectatorPawn.h"

void ABaseHUD::BeginPlay()
{
    Super::BeginPlay();
    
    auto const TempGameState = GetBaseGameState();
    TempGameState->OnMatchStateChangedEvent.AddDynamic(this, &ABaseHUD::GameStateChanged);
    
    CreatePreMatchWidget();
    ShowPreMatchWidget();
}

ABaseGameState* ABaseHUD::GetBaseGameState()
{
    return Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));
}

void ABaseHUD::GameStateChanged(TEnumAsByte<EMatchState> NewMatchState)
{
    if(NewMatchState == EMatchState::Game)
    {
        CreateTabMenu();
        CreateMainWidget();
        DestroyPreMatchWidget();
        ShowMainWidget();
    }
    else if(NewMatchState == EMatchState::MatchEnd)
    {
        CreateMatchOverWidget();
        ShowGameOverMatchWidget();
        HiddenTabMenu();
    }
}

void ABaseHUD::CreateTabMenu()
{
    TabMenuWidget = DataAsset->SyncCreateWidget(GetWorld(), DataAsset->GetWidgetData()->TabMenuWidgetClass, GetOwningPlayerController());
}

void ABaseHUD::CreateMainWidget()
{
    MainWidget = DataAsset->SyncCreateWidget(GetWorld(), DataAsset->GetWidgetData()->MainWidgetClass, GetOwningPlayerController());
}

void ABaseHUD::CreatePreMatchWidget()
{
    PreMatch = DataAsset->SyncCreateWidget(GetWorld(), DataAsset->GetWidgetData()->PreStartWidgetClass, GetOwningPlayerController());
}

void ABaseHUD::CreateMatchOverWidget()
{
    MatchOverWidget = DataAsset->SyncCreateWidget(GetWorld(), DataAsset->GetWidgetData()->MatchOverWidgetClass, GetOwningPlayerController());
}

void ABaseHUD::ShowPreMatchWidget()
{
    if(PreMatch && !PreMatch->IsPendingKill())
    {
        PreMatch->AddToViewport();
    }
    else
    {
        CreatePreMatchWidget();
        PreMatch->AddToViewport();
    } 
}

void ABaseHUD::DestroyPreMatchWidget()
{
    PreMatch->RemoveFromParent();
    PreMatch->ConditionalBeginDestroy();
    PreMatch = nullptr;   
}

void ABaseHUD::ShowTabMenu() 
{
    if(TabMenuWidget && !TabMenuWidget->IsPendingKill())
    {
        TabMenuWidget->AddToViewport();
    }
    else
    {
        CreateTabMenu();
        TabMenuWidget->AddToViewport();
    }    
}

void ABaseHUD::HiddenTabMenu() 
{    
    if(TabMenuWidget && !TabMenuWidget->IsPendingKill())
        TabMenuWidget->RemoveFromParent();   
}

void ABaseHUD::ShowMainWidget() 
{
    if(MainWidget && !MainWidget->IsPendingKill())
    {
        MainWidget->AddToViewport();
    }
    else
    {
        CreateMainWidget();
        MainWidget->AddToViewport();
    }
}

void ABaseHUD::HiddenMainWidget()
{
    if(MainWidget && !MainWidget->IsPendingKill())
        MainWidget->RemoveFromParent();
}

void ABaseHUD::ShowGameOverMatchWidget()
{
    if(MatchOverWidget && !MatchOverWidget->IsPendingKill())
    {
        MatchOverWidget->AddToViewport();
    }
    else
    {
        CreateMatchOverWidget();
        MatchOverWidget->AddToViewport();
    }
}

void ABaseHUD::DestroyMatchOverWidget()
{
    MatchOverWidget->RemoveFromParent();
    MatchOverWidget->ConditionalBeginDestroy();
    MatchOverWidget = nullptr;
}