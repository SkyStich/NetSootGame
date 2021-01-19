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

UCLASS(minimalapi)
class ANetWorkShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

	void GetFreeSpawnPoints(TArray<APlayerStartBase*> & FreePoints);
public:
	
	ANetWorkShooterGameMode();

	/** Change points, kills, deaths... For the killer and the murdered */
	UFUNCTION()
	virtual void UpDateDeathPoints(class ABasePlayerState* LoserState, class ABasePlayerState* InstigatorState);

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

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void MatchTimeEnded();

	/** Called when  match be stop*/
	UFUNCTION()
	virtual void StopGameMatch();
public:

	FMatchStarted MatchStartedEvent;
	FMatchEnded OnMatchStopEvent;

	UPROPERTY(BlueprintAssignable)
	FPlayerDeadEvent PlayerDeadEvent;
	
protected:

	/** All points where can be spawn the player  */
	UPROPERTY()
	TArray<AActor*> AllStartPoints;

	/** Time before respawn the player */
	float RespawnTime;
};



