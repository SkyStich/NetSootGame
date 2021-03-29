// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangeWeaponObject.h"
#include "BaseShotgunObject.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKSHOOTER_API UBaseShotgunObject : public URangeWeaponObject
{
	GENERATED_BODY()

public:

UBaseShotgunObject();

protected:
	
	virtual void DropLineTrace(FHitResult& OutHit) override;
};
