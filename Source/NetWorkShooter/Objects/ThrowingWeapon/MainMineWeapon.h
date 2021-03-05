// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainThrowingWeapon.h"
#include "MainMineWeapon.generated.h"

/**
 * Base Object class for all mines
 */
UCLASS()
class NETWORKSHOOTER_API UMainMineWeapon : public UMainThrowingWeapon
{
	GENERATED_BODY()

	class ABaseMineSpecialWeapon* SettingsMine(AController* OwnerController, const FVector& SpawnLocation);

public:

	virtual bool UseWeapon() override;
	virtual void StopUseWeapon() override;

protected:

	virtual void FindPositionForSettingsMine(AController* OwnerController);
	
	virtual bool IsAbleToUseWeapon() override;
	virtual void OuterDead(AController* Controller) override;
	virtual void PreparationForUse() override;
	
};
