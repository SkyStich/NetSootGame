// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "../NetWorkShooterGameMode.h"
#include "BaseGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMatchTimeIsOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMatchEnd, FString, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMatchStart);

UCLASS(Abstract)
class NETWORKSHOOTER_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()

    /** Add to PlayTime One Second*/
    UFUNCTION()
    void IncrementPlayTime();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastMatchEnd(const FString& Reason);
    void MulticastMatchEnd_Implementation(const FString& Reason);
    
    UFUNCTION(NetMulticast, Reliable)
    void MulticastMatchStart();
    void MulticastMatchStart_Implementation();
    
public:

    ABaseGameState();

    /** Start time for increment One second in PlayTime */
    UFUNCTION()
    void StartGameTimer();

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    UFUNCTION()
    virtual void MatchStart();

    UFUNCTION()
    virtual void MatchEnd(FString Reason);
    
    UFUNCTION(BlueprintPure)
    FTimespan GetCurrentPlayTime() const { return CurrentPlayTime; }  
    
protected:

    virtual void BeginPlay() override;

    /** Game duration */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameState|PlayTime")
    FTimespan MatchDurationTime;
private:

    /** Current game Handle */
    FTimerHandle TimeTickHandle;

    UPROPERTY(Replicated)
    FTimespan CurrentPlayTime;

public:

    UPROPERTY(BlueprintAssignable)
    FMatchTimeIsOver OnMatchTimeIsOverEvent;

    UPROPERTY(BlueprintAssignable)
    FMatchEnd OnMatchEndedEvent;

    UPROPERTY(BlueprintAssignable)
    FMatchStarted OnMatchStartedEvent;
};
