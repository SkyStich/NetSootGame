// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainThrowingWeapon.h"
#include "MainGrenadeObject.generated.h"

UCLASS(Blueprintable)
class NETWORKSHOOTER_API UMainGrenadeObject : public UMainThrowingWeapon
{
	GENERATED_BODY()

	void PreparationForThrow();

	/** Called when its owner dies */
	void OuterDead(AController* Controller);
	
public:

	virtual void StopUseWeapon() override;
	virtual bool UseWeapon() override;

protected:

	void GrenadeThrow(float const TotalTimeBeforeExplosion, bool bThrowSucceeded, AController* Controller);
	virtual void BeginPlay() override;
	
private:

	bool bWantPreparation;
	FTimerHandle ExplosionHandle;
	FTimerHandle PreparationForUseHandle;
};
