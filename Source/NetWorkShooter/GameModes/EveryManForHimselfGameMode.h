// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../NetWorkShooterGameMode.h"
#include "EveryManForHimselfGameMode.generated.h"

UCLASS()
class NETWORKSHOOTER_API AEveryManForHimselfGameMode : public ANetWorkShooterGameMode
{
	GENERATED_BODY()

protected:

		virtual void BeginPlay() override;
	
public:

	virtual void CharacterDead(AController* LoserController, AController* InstigatedBy, AActor* KillingCauser) override;
	virtual void SpawnPlayer(AController* Controller) override;
	virtual void SpawnSpectator(AController* LoserController, AController* DeathInstigator , AMainSpectatorPawn* & NewSpectator) override;
};
