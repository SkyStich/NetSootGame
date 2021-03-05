// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainThrowingWeapon.h"
#include "MainGrenadeObject.generated.h"

UCLASS(Blueprintable)
class NETWORKSHOOTER_API UMainGrenadeObject : public UMainThrowingWeapon
{
	GENERATED_BODY()



	/** Called when its owner dies */
	virtual void OuterDead(AController* Controller) override;
	
public:

	virtual void StopUseWeapon() override;
	virtual bool UseWeapon() override;

protected:

	void GrenadeThrow(float const TotalTimeBeforeExplosion, bool bThrowSucceeded, AController* Controller);
	virtual void BeginPlay() override;
	virtual void PreparationForUse() override;
	
private:

	bool bWantPreparation;
	FTimerHandle ExplosionHandle;
};
