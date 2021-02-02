// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ACommandHUD::ACommandHUD()
{
	ConstructorHelpers::FClassFinder<UUserWidget>TabMenuClassFinder(TEXT("/Game/ThirdPersonCPP/UI/TabMenu/Design/W_CommandTabMenu"));
	ConstructorHelpers::FClassFinder<UUserWidget>MainWidgetClassFinder(TEXT("/Game/ThirdPersonCPP/UI/HUD/W_HUD_MainWidget"));
    
	if(TabMenuClassFinder.Succeeded() && MainWidgetClassFinder.Succeeded())
	{
		TabMenuWidgetClass = TabMenuClassFinder.Class;
		MainWidgetClass = MainWidgetClassFinder.Class;
	}
}