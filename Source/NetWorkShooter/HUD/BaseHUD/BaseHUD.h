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
    
    void CreateTabMenu();
    void CreateMainWidget();
    void CreatePreMatchWidget();
    void CreateMatchOverWidget();
public:
  
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowTabMenu();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void HiddenTabMenu();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowMainWidget();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void HiddenMainWidget();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowPreMatchWidget();

    /** remove widget in viewport and from memory */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void DestroyPreMatchWidget();
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowGameOverMatchWidget();

    /** remove widget in viewport and from memory */
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void DestroyMatchOverWidget();

    UUserWidget* GetMainWidget() const { return MainWidget; }

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
