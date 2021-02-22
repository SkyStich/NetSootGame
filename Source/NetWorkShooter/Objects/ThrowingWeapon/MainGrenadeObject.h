// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainThrowingWeapon.h"
#include "MainGrenadeObject.generated.h"

UCLASS(Blueprintable)
class NETWORKSHOOTER_API UMainGrenadeObject : public UMainThrowingWeapon
{
	GENERATED_BODY()

protected:

	void GrenadeThrow();
	
};
