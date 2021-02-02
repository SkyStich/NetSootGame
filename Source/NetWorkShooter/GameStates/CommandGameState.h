// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameState.h"
#include "NetWorkShooter/GameModes/CommandGameMode.h"

#include "CommandGameState.generated.h"

/**
 The class is responsible for the state of the command battle. Upon reaching the maximum of one of the commands, the game is considered over.
 The winners are the ones who reach the maximum kills first.
 */

UCLASS()
class NETWORKSHOOTER_API ACommandGameState : public ABaseGameState
{
	GENERATED_BODY()

public:

	ACommandGameState();
	
	UFUNCTION(BlueprintPure)
	int32 GetPointByTeam(TEnumAsByte<ETeamList> Team);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	void ChangeTeamPoints(const TEnumAsByte<ETeamList>& Team, int32 Value);

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(Replicated)
	TMap<TEnumAsByte<ETeamList>, int32> TeamPoints; 
};
