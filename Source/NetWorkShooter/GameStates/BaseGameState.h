// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "NetWorkShooter/Components/ChatComponent.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewPlayerConnected, ABasePlayerState*, NewPlayer);

class ANetWorkShooterGameMode;

UCLASS(Abstract)
class NETWORKSHOOTER_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()

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
    
    UFUNCTION(BlueprintPure, Category = "GameState|Time")
    FTimespan GetCurrentPlayTime() const { return CurrentPlayTime; }  
    
    UFUNCTION(BlueprintPure, Category = "GameState|MatchState")
    TEnumAsByte<EMatchState> GetMatchState() const { return MatchState; }

    UFUNCTION(BlueprintPure, Category = "GameState|Time")
    int32 GetTimeBeforeStartOfMatch() const { return TimeBeforeStartOfMatch; }

    UFUNCTION(BlueprintPure, Category = "GameState|Chat")
    UChatComponent* GetChatComponent() const { return ChatComponent; }

    void NewPlayerConnected(ABasePlayerState* NewPlayer);
    
protected:

    virtual void BeginPlay() override;
    
    UFUNCTION(NetMulticast, Reliable)
    void MulticastMatchEnd(const FString& Reason);
    virtual void MulticastMatchEnd_Implementation(const FString& Reason);
    
    UFUNCTION(NetMulticast, Reliable)
    void MulticastMatchStart();
    void MulticastMatchStart_Implementation();
    
private:

    UPROPERTY(Replicated)
    FTimespan CurrentPlayTime;

    UPROPERTY(EditAnywhere, Category = "TimeBeforeStartGame")
    int32 MaxTimeBeforeStartOfMatch;

    FTimerHandle GameStartTimer;

    UPROPERTY(VisibleAnywhere)
    UChatComponent* ChatComponent;

public:

    UPROPERTY(BlueprintAssignable)
    FMatchEnd OnMatchEndedEvent;

    UPROPERTY(BlueprintAssignable)
    FMatchStart OnMatchStartedEvent;
    
    UPROPERTY(BlueprintAssignable)
    FNewPlayerConnected OnNewPlayerConnectedEvent;

    FMatchStateChanged OnMatchStateChangedEvent;

protected:

    UPROPERTY(ReplicatedUsing = OnRep_MatchState)
    TEnumAsByte<EMatchState> MatchState;

    UPROPERTY(Replicated)
    int32 TimeBeforeStartOfMatch;

    /** Game duration */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameState|PlayTime")
    FTimespan MatchDurationTime;
};
