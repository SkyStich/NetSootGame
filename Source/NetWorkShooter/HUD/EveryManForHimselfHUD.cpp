// Fill out your copyright notice in the Description page of Project Settings.


#include "EveryManForHimselfHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AEveryManForHimselfHUD::AEveryManForHimselfHUD()
{
	ConstructorHelpers::FObjectFinder<UWidgetsDataAsset>EveryManDataAssetFinder(TEXT("/Game/ThirdPersonCPP/Blueprints/DataAssets/WIdgetAssets/EveryManForHimselfWidgetData"));
    
	if(EveryManDataAssetFinder.Succeeded())
	{
		DataAsset = EveryManDataAssetFinder.Object;
	}
}
