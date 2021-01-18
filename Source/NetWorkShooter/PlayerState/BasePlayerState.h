// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKSHOOTER_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:

    ABasePlayerState();
    
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

    /** Increment and Decrement Number of kills for the whole match */
    void IncrementNumberOfMurders() { NumberOfMurders++; }
    void DecrementNumberOfMurders() { NumberOfMurders--; }

    /** Increment of kills for the whole match */
    void IncrementNumberOfDeaths() { NumberOfDeaths++; }

    int32 GetNumberOfMurders() const { return  NumberOfMurders; }

private:

    /** Number of kills for the whole match */
    UPROPERTY(Replicated)
    int32 NumberOfMurders;

    /** Number of deaths for the whole match */
    UPROPERTY(Replicated)
    int32 NumberOfDeaths;
};
