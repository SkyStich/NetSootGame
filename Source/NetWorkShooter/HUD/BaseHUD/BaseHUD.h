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

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void DestroyPreMatchWidget();
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowGameOverMatchWidget();
    
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void DestroyMatchOverWidget();

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
