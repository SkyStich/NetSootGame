// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Controllers/PlayerControllers/MainPlayerController.h"
#include "NetWorkShooterCharacter.h"
#include "Spectators/MainSpectatorPawn.h"
#include "NetWorkShooterGameMode.generated.h"

class APlayerStartBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPlayerDeadEvent, AController*, LoserController, AController*, InstigatedBy, AActor*, KillingCauser);

/** Bind when the match has started directly (All players are ready and can perform actions)*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMatchStarted);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMatchEnded, FString, StopReason);

UCLASS(minimalapi, Abstract)
class ANetWorkShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

	void StartGameTimer();
	
public:
	
	ANetWorkShooterGameMode();
	
	UFUNCTION(BlueprintPure, Category = "GameMode|Getting")
	bool GetCanAutoRespawn() const { return bCanAutoRespawn; }
	
	UFUNCTION(BlueprintPure, Category = "GameMode|Getting")
	float GetRespawnTime() const { return RespawnTime; }

	/** Is responsible for calculating death and killing points */
	/** Return false if InstigatorController is empty */
	UFUNCTION()
	virtual bool UpDateDeathPoints(AController* LoserController, AController* InstigatorController);

	/** Called when a player dies */
	UFUNCTION()
	virtual void CharacterDead(AController* LoserController, AController* DeathInstigator, AActor* KillingCauser);

	/** Responsible for a player spawned */
	UFUNCTION()
	virtual void SpawnPlayer(AController* Controller);

	/** Responsible for the spectator spawned */
	UFUNCTION()
	virtual void SpawnSpectator(AController* LoserController, AController* DeathInstigator , AMainSpectatorPawn* & NewSpectator) {  }

	/** The function is called when the match has started directly (All players are ready and can perform actions) */
	UFUNCTION()
	virtual void StartGameMatch();

	/** Called when  match be stop*/
	UFUNCTION()
    virtual void StopGameMatch(FString StopReason);
	
protected:

	virtual void BeginPlay() override;

	/** Return an array of points for the player to spawn with conditions from the function */
	void GetFreeSpawnPoints(TArray<APlayerStartBase*> & FreePoints, AController* SpawnController);
	
	/** Function responsible for the conditions for selecting spawn points */
	virtual bool PointSelectionConditions(AController* SpawnController, APlayerStartBase* PointToCheck);
public:

	FMatchStarted MatchStartedEvent;
	FMatchEnded OnMatchStopEvent;

	UPROPERTY(BlueprintAssignable)
	FPlayerDeadEvent PlayerDeadEvent;
protected:

	/** All points where can be spawn the player  */
	UPROPERTY()
	TArray<AActor*> AllStartPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanAutoRespawn;

	/** Time before respawn the player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RespawnTime;

private:

	/** Current game Handle */
	FTimerHandle TimeTickHandle;
	FTimerDelegate TimerDelegate;
};



