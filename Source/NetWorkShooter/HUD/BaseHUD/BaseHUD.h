// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NetWorkShooter/GameStates/BaseGameState.h"
#include "NetWorkShooter/DataAssets/WidgetsDataAsset.h"
#include "BaseHUD.generated.h"

class UUserWidget;

UCLASS(Abstract)
class NETWORKSHOOTER_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

    /** Helpers get base game state */
    ABaseGameState* GetBaseGameState();
    
protected:

    UFUNCTION()
    void GameStateChanged(TEnumAsByte<EMatchState> NewMatchState);

    void CreateWidget(UUserWidget*& Widget, TSoftClassPtr<UUserWidget> WidgetClass);
public:
  
    virtual void BeginPlay() override;
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void AddToViewportWidget(UUserWidget* Widget);

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void HiddenTabMenu();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void HiddenMainWidget();

    /** remove widget in viewport and from memory */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void DestroyPreMatchWidget();

    /** remove widget in viewport and from memory */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void DestroyMatchOverWidget();

    UUserWidget* GetMainWidget() const { return MainWidget; }
    UUserWidget* GetTabMenuWidget() const { return TabMenuWidget; }

protected:

    UPROPERTY()
    UUserWidget* TabMenuWidget;

    UPROPERTY()
    UUserWidget* MainWidget;

    UPROPERTY()
    UUserWidget* PreMatch;

    UPROPERTY()
    UUserWidget* MatchOverWidget;
    
    UPROPERTY()
    UWidgetsDataAsset* DataAsset;
};
