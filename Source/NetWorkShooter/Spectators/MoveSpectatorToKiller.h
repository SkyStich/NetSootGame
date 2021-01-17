// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainSpectatorPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MoveSpectatorToKiller.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKSHOOTER_API AMoveSpectatorToKiller : public AMainSpectatorPawn
{
	GENERATED_BODY()

public:
	
	AMoveSpectatorToKiller();
	void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;
};
