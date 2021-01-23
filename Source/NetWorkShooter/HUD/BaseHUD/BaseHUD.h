// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UUserWidget;

UCLASS(Blueprintable)
class NETWORKSHOOTER_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
protected:

    UFUNCTION()
    void MatchStarted();
    
    void CreateTabMenu();
    void CreateMainWidget();
    
public:

    ABaseHUD();
  
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowTabMenu() const;

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void HiddenTabMenu() const;

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowMainWidget() const;

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void HiddenMainWidget() const;

private:

    TSubclassOf<UUserWidget> TabMenuWidgetClass;
    TSubclassOf<UUserWidget> MainWidgetClass;

    UUserWidget* TabMenuWidget;
    UUserWidget* MainWidget;
};
