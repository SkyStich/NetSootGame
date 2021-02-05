// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "NetWorkShooter/Spectators/MainSpectatorPawn.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"
#include "Engine/Font.h"

void ABaseHUD::BeginPlay()
{
    Super::BeginPlay();
    
    GetBaseGameState()->OnMatchStateChangedEvent.AddDynamic(this, &ABaseHUD::MatchStarted);
    CreatePreMatchWidget();
    ShowPreMatchWidget();
}

ABaseGameState* ABaseHUD::GetBaseGameState()
{
    return Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));
}

void ABaseHUD::MatchStarted(TEnumAsByte<EMatchState> NewMatchState)
{
    if(NewMatchState == EMatchState::Game)
    {
        CreateTabMenu();
        CreateMainWidget();
        ShowMainWidget();
        DestroyPreMatchWidget();
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
    if(GetBaseGameState()->GetMatchState() == EMatchState::Game)
    {
        PreMatch->RemoveFromParent();
        PreMatch->ConditionalBeginDestroy();
        PreMatch = nullptr;
    }
}

void ABaseHUD::ShowTabMenu() 
{
    if(GetBaseGameState()->GetMatchState() == EMatchState::Game)
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
}

void ABaseHUD::HiddenTabMenu() 
{
    if(GetBaseGameState()->GetMatchState() == EMatchState::Game)
    {
        if(TabMenuWidget && !TabMenuWidget->IsPendingKill())
            TabMenuWidget->RemoveFromParent();
    }
}

void ABaseHUD::ShowMainWidget() 
{
    if(GetBaseGameState()->GetMatchState() == EMatchState::Game)
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
}

void ABaseHUD::HiddenMainWidget()
{
    if(GetBaseGameState()->GetMatchState() == EMatchState::Game)
    {
        if(MainWidget && !MainWidget->IsPendingKill())
            MainWidget->RemoveFromParent();
    }
}