// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BaseGameState.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKSHOOTER_API ABaseGameState : public AGameState
{
	GENERATED_BODY()

    /** Add to PlayTime One Second*/
    UFUNCTION()
    void IncrementPlayTime();
    
public:

    /** Start time for increment One second in PlayTime */
    UFUNCTION()
    void StartGameTimer();

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
    
protected:

    /** Game duration */
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState|PlayTime")
    FTimespan PlayTime;

private:

    FTimerHandle TimeTickHandle;
};
