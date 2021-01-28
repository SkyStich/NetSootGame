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
	virtual void UpdateTheKillCounter(AController* LoserController, AController* DeathInstigator, AActor* KillingCauser) override;

	UFUNCTION(BlueprintPure, Category = "GameMode|EveryManForHimself")
	int32 GetCurrentKillInMatch() const { return CurrentKillInMatch; }

protected:

	/** If = 0, match have Not killLimit */
	UPROPERTY(EditAnywhere)
	int32 MatchWithKillLimit;

private:
	
	UPROPERTY(Replicated)
	int32 CurrentKillInMatch;
};
