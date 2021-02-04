// Fill out your copyright notice in the Description page of Project Settings.


#include "EveryManForHimselfHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AEveryManForHimselfHUD::AEveryManForHimselfHUD()
{
	ConstructorHelpers::FClassFinder<UUserWidget>TabMenuClassFinder(TEXT("/Game/ThirdPersonCPP/UI/TabMenu/Design/W_TabMenu"));
	ConstructorHelpers::FClassFinder<UUserWidget>MainWidgetClassFinder(TEXT("/Game/ThirdPersonCPP/UI/HUD/W_HUD_MainWidget"));
	ConstructorHelpers::FObjectFinder<UWidgetsDataAsset>EveryManDataAssetFinder(TEXT("/Game/ThirdPersonCPP/Blueprints/DataAssets/WIdgetAssets/EveryManForHimselfWidgetData"));
    
	if(TabMenuClassFinder.Succeeded() && MainWidgetClassFinder.Succeeded())
	{
		TabMenuWidgetClass = TabMenuClassFinder.Class;
		MainWidgetClass = MainWidgetClassFinder.Class;
	}
	if(EveryManDataAssetFinder.Succeeded())
	{
		DataAsset = EveryManDataAssetFinder.Object;
	}
}
