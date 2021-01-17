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

protected:

    UPROPERTY(BlueprintReadOnly, Replicated, Category = State)
    int32 NumberOfMurders;
    
    UPROPERTY(BlueprintReadOnly, Replicated, Category = State)
    int32 NumberOfDeaths;
};
