// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "NetWorkShooter/GameStates/BaseGameState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Font.h"
#include "UObject/ConstructorHelpers.h"

ABaseHUD::ABaseHUD()
{

}

void ABaseHUD::BeginPlay()
{
    Super::BeginPlay();
    
    Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()))->OnMatchStartedEvent.AddDynamic(this, &ABaseHUD::MatchStarted);
}

void ABaseHUD::DrawHUD()
{
    Super::DrawHUD();

    auto const State = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));
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
        FString Min("");
        FString Sec("");
        Minutes < 10 ? Min = "0" + FString::FromInt(Minutes) : Min = FString::FromInt(Minutes);
        Seconds < 10 ? Sec = "0" + FString::FromInt(Seconds) : Sec = FString::FromInt(Seconds);

        /** Draw text */
        DrawText(Min + ":" + Sec, FColor::Cyan, ScreenX, ScreenY, 0, 1.5f, false);
    }
}

void ABaseHUD::MatchStarted()
{
    CreateTabMenu();
    CreateMainWidget();
}

void ABaseHUD::CreateTabMenu()
{
    TabMenuWidget = CreateWidget(GetOwningPlayerController(), TabMenuWidgetClass, "TabMenu");
}

void ABaseHUD::CreateMainWidget()
{
    MainWidget = CreateWidget(GetOwningPlayerController(), MainWidgetClass, "MainWidget");
    MainWidget->AddToViewport();
}

void ABaseHUD::ShowTabMenu() const
{
    if(TabMenuWidget && !TabMenuWidget->IsPendingKill())
        TabMenuWidget->AddToViewport();
}

void ABaseHUD::HiddenTabMenu() const
{
    if(TabMenuWidget && !TabMenuWidget->IsPendingKill())
        TabMenuWidget->RemoveFromParent();
}

void ABaseHUD::ShowMainWidget() const
{
    if(MainWidget && !MainWidget->IsPendingKill())
        MainWidget->SetVisibility(ESlateVisibility::Visible);
}

void ABaseHUD::HiddenMainWidget() const
{
    if(MainWidget && !MainWidget->IsPendingKill())
        MainWidget->SetVisibility(ESlateVisibility::Collapsed);
}

