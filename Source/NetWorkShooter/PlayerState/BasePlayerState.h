// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateStateKDA);

UCLASS()
class NETWORKSHOOTER_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

    UFUNCTION()
    void OnRep_KDA() { OnUpdateStateKDAEvent.Broadcast(); }
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

private:

    /** Number of kills for the whole match */
    UPROPERTY(ReplicatedUsing = OnRep_KDA)
    int32 NumberOfMurders;

    /** Number of deaths for the whole match */
    UPROPERTY(ReplicatedUsing = OnRep_KDA)
    int32 NumberOfDeaths;

public:

    UPROPERTY(BlueprintAssignable)
    FUpdateStateKDA OnUpdateStateKDAEvent;
};
