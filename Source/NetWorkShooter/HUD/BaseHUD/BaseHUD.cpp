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
    
    CreateWidget(PreMatch, DataAsset->GetWidgetData()->PreStartWidgetClass);
    AddToViewportWidget(PreMatch);
}

ABaseGameState* ABaseHUD::GetBaseGameState()
{
    return Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));
}

void ABaseHUD::GameStateChanged(TEnumAsByte<EMatchState> NewMatchState)
{
    if(NewMatchState == EMatchState::Game)
    {
        CreateWidget(TabMenuWidget, DataAsset->GetWidgetData()->TabMenuWidgetClass);
        CreateWidget(MainWidget, DataAsset->GetWidgetData()->MainWidgetClass);
        DestroyPreMatchWidget();
        AddToViewportWidget(MainWidget);
    }
    else if(NewMatchState == EMatchState::MatchEnd)
    {
        CreateWidget(MatchOverWidget, DataAsset->GetWidgetData()->MatchOverWidgetClass);
        AddToViewportWidget(MatchOverWidget);
        HiddenTabMenu();
    }
}

void ABaseHUD::CreateWidget(UUserWidget*& Widget, TSoftClassPtr<UUserWidget> WidgetClass)
{
   Widget = DataAsset->SyncCreateWidget(GetWorld(), WidgetClass, GetOwningPlayerController());
}

void ABaseHUD::AddToViewportWidget(UUserWidget* Widget)
{
    if(Widget && !Widget->IsPendingKill())
    {
        Widget->AddToViewport();
    }
}

void ABaseHUD::DestroyPreMatchWidget()
{
    PreMatch->RemoveFromParent();
    PreMatch->ConditionalBeginDestroy();
    PreMatch = nullptr;   
}

void ABaseHUD::HiddenTabMenu() 
{    
    if(TabMenuWidget && !TabMenuWidget->IsPendingKill())
        TabMenuWidget->RemoveFromParent();   
}

void ABaseHUD::HiddenMainWidget()
{
    if(MainWidget && !MainWidget->IsPendingKill())
        MainWidget->RemoveFromParent();
}

void ABaseHUD::DestroyMatchOverWidget()
{
    MatchOverWidget->RemoveFromParent();
    MatchOverWidget->ConditionalBeginDestroy();
    MatchOverWidget = nullptr;
}

