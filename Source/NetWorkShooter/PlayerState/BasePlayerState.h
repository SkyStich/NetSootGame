// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

UENUM(BlueprintType)
enum ECauseOfPlayerDeath
{
    EnemyKilledEnemy, //Base death
    Suicide, //The player is directly involved in his death
    FriendlyKilled, // Player killed a player from his team
    DeathByNegligence //For example the player crashed
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateStateKDA);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPlayerDeadEvent, const FString&, LoserName, const FString&, InstigatedName, const FString&, WeaponName);

UCLASS()
class NETWORKSHOOTER_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

    UFUNCTION()
    void OnRep_KDA() const { OnUpdateStateKDAEvent.Broadcast(); }
    
public:
    
    ABasePlayerState();
    
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    /** Increment and Decrement Number of kills for the whole match */
    UFUNCTION()
    void IncrementNumberOfMurders();

    UFUNCTION()
    void DecrementNumberOfMurders();
    
    /** Increment of kills for the whole match */
    UFUNCTION()
    void IncrementNumberOfDeaths();
    
    UFUNCTION(BlueprintPure)
    int32 GetNumberOfMurders() const { return  NumberOfMurders; }
    
    UFUNCTION(BlueprintPure)
    int32 GetNumberOfDeaths() const { return  NumberOfDeaths; }

    
    UFUNCTION(NetMulticast, Unreliable)
    void NetMulticastOwnerDead(const FString& LoserName, const FString& InstigatorName, const FString& WeaponName);

    void SetIsAlive(bool const bNewState);

protected:

    virtual void BeginPlay() override;

private:

    /** Number of kills for the whole match */
    UPROPERTY(ReplicatedUsing = OnRep_KDA)
    int32 NumberOfMurders;

    /** Number of deaths for the whole match */
    UPROPERTY(ReplicatedUsing = OnRep_KDA)
    int32 NumberOfDeaths;

    /* return true in owner player character is alive **/
    UPROPERTY(Replicated)
    bool bIsAlive;
    
    /** Only test */
    TArray<FString>PlayersName;

public:

    UPROPERTY(BlueprintAssignable)
    FUpdateStateKDA OnUpdateStateKDAEvent;
    
    UPROPERTY(BlueprintAssignable)
    FPlayerDeadEvent OnPlayerDeadEvent;
};
