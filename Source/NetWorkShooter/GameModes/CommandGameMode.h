// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetWorkShooter/NetWorkShooterGameMode.h"
#include "CommandGameMode.generated.h"

UENUM(BlueprintType)
enum ETeamList
{
    TeamA,
    TeamB,
	TeamNone
};

UCLASS(Abstract)
class NETWORKSHOOTER_API ACommandGameMode : public ANetWorkShooterGameMode
{
	GENERATED_BODY()

public:

   ACommandGameMode();

    virtual void SpawnSpectator(AController* LoserController, AController* DeathInstigator, AMainSpectatorPawn*& NewSpectator) override;

	/** Is responsible for awarding points to certain trams */
    virtual bool UpDateDeathPoints(AController* LoserController, AController* InstigatorController) override;
	virtual void StartGameMatch() override;
	
protected:

    virtual void BeginPlay() override;
	virtual bool PointSelectionConditions(AController* SpawnController, APlayerStartBase* PointToCheck) override;
	
	/** Auto balance of teams  in command mode*/
	virtual void AutoBalanceOfTeams();

private:

	/** Be responsible for the maximum number of kills. */
	UPROPERTY(EditAnywhere, Category = "CommandState|Kills")
	int32 MaximumCountOfKillsPerMatch;
};
