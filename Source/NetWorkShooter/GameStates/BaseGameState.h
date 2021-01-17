// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BaseGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMatchTimeIsOver);

UCLASS()
class NETWORKSHOOTER_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()

    /** Add to PlayTime One Second*/
    UFUNCTION()
    void IncrementPlayTime();
    
public:

    ABaseGameState();

    /** Start time for increment One second in PlayTime */
    UFUNCTION()
    void StartGameTimer();

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
    
protected:

    virtual void BeginPlay() override;

    /** Game duration */
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "GameState|PlayTime")
    FTimespan PlayTime;

private:

    FTimerHandle TimeTickHandle;

public:

    UPROPERTY(BlueprintAssignable)
    FMatchTimeIsOver MatchTimeIsOverEvent;
};
