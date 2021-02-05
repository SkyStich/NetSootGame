// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandHUD.h"
#include "Blueprint/UserWidget.h"
#include "NetWorkShooter/DataAssets/WidgetsDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ACommandHUD::ACommandHUD()
{
	ConstructorHelpers::FObjectFinder<UWidgetsDataAsset>WidgetDataAssetFinder(TEXT("/Game/ThirdPersonCPP/Blueprints/DataAssets/WIdgetAssets/CommandWidgetData"));
	if(WidgetDataAssetFinder.Succeeded())
	{
		DataAsset = WidgetDataAssetFinder.Object;
	}
}