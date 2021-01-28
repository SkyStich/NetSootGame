// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "../NetWorkShooterGameMode.h"
#include "EveryManForHimselfGameMode.generated.h"

UCLASS(Abstract)
class NETWORKSHOOTER_API AEveryManForHimselfGameMode : public ANetWorkShooterGameMode
{
	GENERATED_BODY()

public:

	AEveryManForHimselfGameMode();
	
	virtual void SpawnSpectator(AController* LoserController, AController* DeathInstigator , AMainSpectatorPawn* & NewSpectator) override;

protected:

	virtual void BeginPlay() override;
};
