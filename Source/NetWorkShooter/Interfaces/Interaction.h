// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../NetWorkShooterCharacter.h"
#include "Interaction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NETWORKSHOOTER_API IInteraction
{
	GENERATED_BODY()

	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	bool InteractWithActor(ANetWorkShooterCharacter* OwnerInteraction);
	
};
