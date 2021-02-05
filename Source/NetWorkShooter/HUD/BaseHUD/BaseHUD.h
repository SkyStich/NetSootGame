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
    void MatchStarted(TEnumAsByte<EMatchState> NewMatchState);
    
    void CreateTabMenu();
    void CreateMainWidget();
    void CreatePreMatchWidget();
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

protected:

    UPROPERTY()
    UUserWidget* TabMenuWidget;

    UPROPERTY()
    UUserWidget* MainWidget;

    UPROPERTY()
    UUserWidget* PreMatch;

    UPROPERTY()
    UWidgetsDataAsset* DataAsset;
};
