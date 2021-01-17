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

UCLASS(minimalapi)
class ANetWorkShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	void GetFreeSpawnPoints(TArray<APlayerStartBase*> & FreePoints);

public:
	
	ANetWorkShooterGameMode();

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
	void StartGameMatch();

protected:

	void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable)
	FMatchStarted MatchStartedEvent;

	FPlayerDeadEvent PlayerDeadEvent;

protected:

	UPROPERTY()
	TArray<AActor*> AllStartPoints;

	/** Can auto respawn player's */
	bool bAutoRespawn;

	/** Time before respawn the player */
	float RespawnTime;
};



