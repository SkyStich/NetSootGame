// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseGameState.generated.h"

UENUM(BlueprintType)
enum EMatchState
{
    PreStart,
    Game,
    MatchEnd
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMatchEnd, FString, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMatchStart);

/** Using for sync load user widget after character initialization */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMatchStateChanged, TEnumAsByte<EMatchState>, NewMatchState);

class ANetWorkShooterGameMode;

UCLASS(Abstract)
class NETWORKSHOOTER_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()

    UFUNCTION(NetMulticast, Reliable)
    void MulticastMatchEnd(const FString& Reason);
    void MulticastMatchEnd_Implementation(const FString& Reason);
    
    UFUNCTION(NetMulticast, Reliable)
    void MulticastMatchStart();
    void MulticastMatchStart_Implementation();

    void GameStartTime();

    UFUNCTION()
    void OnRep_MatchState();
    
public:

    ABaseGameState();

    /** Add to PlayTime One Second*/
    void IncrementPlayTime();

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    UFUNCTION()
    virtual void MatchStart();

    UFUNCTION()
    virtual void MatchEnd(FString Reason);
    
    UFUNCTION(BlueprintPure)
    FTimespan GetCurrentPlayTime() const { return CurrentPlayTime; }  
    
    UFUNCTION(BlueprintPure)
    TEnumAsByte<EMatchState> GetMatchState() const { return MatchState; }

    UFUNCTION(BlueprintPure)
    FTimespan GetTimeBeforeStartOfMatch() const { return TimeBeforeStartOfMatch; }
    
protected:

    virtual void BeginPlay() override;
    
private:

    UPROPERTY(Replicated)
    FTimespan CurrentPlayTime;

    FTimerHandle GameStartTimer;

public:

    UPROPERTY(BlueprintAssignable)
    FMatchEnd OnMatchEndedEvent;

    UPROPERTY(BlueprintAssignable)
    FMatchStart OnMatchStartedEvent;

    FMatchStateChanged OnMatchStateChangedEvent;

protected:

    UPROPERTY(ReplicatedUsing = OnRep_MatchState)
    TEnumAsByte<EMatchState> MatchState;

    UPROPERTY(Replicated)
    FTimespan TimeBeforeStartOfMatch;

    /** Game duration */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameState|PlayTime")
    FTimespan MatchDurationTime;
};
