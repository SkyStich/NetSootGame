// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameState.h"
#include "EveryManForHimselfGameState.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKSHOOTER_API AEveryManForHimselfGameState : public ABaseGameState
{
	GENERATED_BODY()

public:

	AEveryManForHimselfGameState();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	void IncrementCurrentKillInMatch() { CurrentKillInMatch++; }

	UFUNCTION(BlueprintPure, Category = "GameMode|EveryManForHimself")
	int32 GetCurrentKillInMatch() const { return CurrentKillInMatch; }
private:
	
	UPROPERTY(Replicated)
	int32 CurrentKillInMatch;
};
