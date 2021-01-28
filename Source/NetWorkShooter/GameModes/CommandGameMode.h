// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetWorkShooter/NetWorkShooterGameMode.h"
#include "CommandGameMode.generated.h"

UENUM(BlueprintType)
enum ETeamList
{
    TeamNone,
    TeamA,
    TeamB
};

UCLASS(Abstract)
class NETWORKSHOOTER_API ACommandGameMode : public ANetWorkShooterGameMode
{
	GENERATED_BODY()

public:

   ACommandGameMode();

   virtual void SpawnSpectator(AController* LoserController, AController* DeathInstigator, AMainSpectatorPawn*& NewSpectator) override;
   
protected:

    virtual void BeginPlay() override;
	virtual bool PointSelectionConditions(AController* SpawnController, APlayerStartBase* PointToCheck) override;
};
