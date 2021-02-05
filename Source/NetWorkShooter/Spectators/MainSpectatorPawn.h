// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainSpectatorPawn.generated.h"


UCLASS(BlueprintType)
class NETWORKSHOOTER_API AMainSpectatorPawn : public APawn
{
	GENERATED_BODY()

public:

	AMainSpectatorPawn();

protected:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();

	/** Turn controller */
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

protected:
	
	float BaseTurnRate;
	float BaseLookUpRate;
};
