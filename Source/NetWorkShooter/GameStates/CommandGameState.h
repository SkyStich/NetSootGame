// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameState.h"
#include "NetWorkShooter/GameModes/CommandGameMode.h"
#include "CommandGameState.generated.h"
 
/** Called if team points be changed  */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTeamPointChanged);

/**
 *The class is responsible for the state of the command battle. Upon reaching the maximum of one of the commands, the game is considered over.
 *The winners are the ones who reach the maximum kills first.
*/
UCLASS()
class NETWORKSHOOTER_API ACommandGameState : public ABaseGameState
{
	GENERATED_BODY()
	
	UFUNCTION()
	void OnRep_TeamPoints();

public:

	ACommandGameState();
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	/** Increment points of one of the commands using of team list */
	virtual void ChangeTeamPoints(const TEnumAsByte<ETeamList>& Team);

	/** Get points command using of team list */
	UFUNCTION(BlueprintPure)
	int32 GetPointByTeam(TEnumAsByte<ETeamList> Team);

	/** Check points all commands and get winner team */
	UFUNCTION(BlueprintPure)
	TEnumAsByte<ETeamList> CheckWinnerTeam();

private:

	/** An array that stores the scores of both teams. To get the command index, an enumeration(ETeamList(CommandGameMode)) with the command names is used */
	UPROPERTY(ReplicatedUsing = OnRep_TeamPoints)
	TArray<int32>Points;

public:

	UPROPERTY(BlueprintAssignable, Category = "GameState|CommandState")
	FTeamPointChanged OnTeamPointChangedEvent;
};
