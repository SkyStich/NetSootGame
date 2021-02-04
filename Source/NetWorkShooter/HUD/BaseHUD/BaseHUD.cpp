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
}

ABaseGameState* ABaseHUD::GetBaseGameState()
{
    return Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));
}

void ABaseHUD::DrawHUD()
{
    Super::DrawHUD();

    auto const State = GetBaseGameState();
    if(State && !State->IsPendingKill())
    {
        int32 ScreenX, ScreenY;
        GetOwningPlayerController()->GetViewportSize(ScreenX, ScreenY);

        /** Format position for draw text */
        ScreenX /= 2;
        ScreenY /= ScreenX + 40;

        /** Set local var minutes and sec */
        int32 const Minutes = State->GetCurrentPlayTime().GetMinutes();
        int32 const Seconds = State->GetCurrentPlayTime().GetSeconds();
    
        /** Format text */
        FString Min;
        FString Sec;
        Minutes < 10 ? Min = "0" + FString::FromInt(Minutes) : Min = FString::FromInt(Minutes);
        Seconds < 10 ? Sec = "0" + FString::FromInt(Seconds) : Sec = FString::FromInt(Seconds);

        /** Draw text */
        DrawText(Min + ":" + Sec, FColor::Cyan, ScreenX, ScreenY, 0, 1.5f, false);
    }
}

void ABaseHUD::MatchStarted(TEnumAsByte<EMatchState> NewMatchState)
{
    if(NewMatchState == EMatchState::Game)
    {
        CreateTabMenu();
        CreateMainWidget();
        ShowMainWidget(); 
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
        TabMenuWidget && !TabMenuWidget->IsPendingKill() ? TabMenuWidget->RemoveFromParent() : CreateTabMenu();
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
        MainWidget && !MainWidget->IsPendingKill() ? MainWidget->RemoveFromParent() : CreateMainWidget();
    }
}

