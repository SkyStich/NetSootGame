// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpecialWeapon.h"
#include "BaseGrenadeWeapon.generated.h"

UCLASS(Abstract)
class NETWORKSHOOTER_API ABaseGrenadeWeapon : public ABaseSpecialWeapon
{
	GENERATED_BODY()

public:
	ABaseGrenadeWeapon();
	
	void Init(struct FThrowWeaponData* SpecialWeaponData, float const TimeBeforeDetonation, bool bThrowSucceeded);

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere)
	float ForceImpulse;
};
