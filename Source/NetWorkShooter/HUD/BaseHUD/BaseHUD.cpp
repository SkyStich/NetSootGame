// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "NetWorkShooter/GameStates/BaseGameState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ABaseHUD::ABaseHUD()
{
    ConstructorHelpers::FClassFinder<UUserWidget>TabMenuClassFinder(TEXT("/Game/ThirdPersonCPP/UI/TabMenu/Design/W_TabMenu"));
    ConstructorHelpers::FClassFinder<UUserWidget>MainWidgetClassFinder(TEXT("/Game/ThirdPersonCPP/UI/HUD/W_HUD_MainWidget"));
    
    if(TabMenuClassFinder.Succeeded() && MainWidgetClassFinder.Succeeded())
    {
         TabMenuWidgetClass = TabMenuClassFinder.Class;
         MainWidgetClass = MainWidgetClassFinder.Class;
    }
}

void ABaseHUD::BeginPlay()
{
    Super::BeginPlay();
    
    Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()))->OnMatchStartedEvent.AddDynamic(this, &ABaseHUD::MatchStarted);
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
  //  TabMenuWidget->SetVisibility(ESlateVisibility::Visible);
    TabMenuWidget->AddToViewport();
}

void ABaseHUD::HiddenTabMenu() const
{
   // TabMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
   TabMenuWidget->RemoveFromParent();
}

void ABaseHUD::ShowMainWidget() const
{
    MainWidget->SetVisibility(ESlateVisibility::Visible);
}

void ABaseHUD::HiddenMainWidget() const
{
    MainWidget->SetVisibility(ESlateVisibility::Collapsed);
}

