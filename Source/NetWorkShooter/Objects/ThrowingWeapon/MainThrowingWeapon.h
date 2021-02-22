// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../WeaponObject/MainWeaponObject.h"
#include "MainThrowingWeapon.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class NETWORKSHOOTER_API UMainThrowingWeapon : public UMainWeaponObject
{
	GENERATED_BODY()

public:

	virtual void Init(UDataTable* WeaponData, TCHAR* ContextString) override;

	virtual TAssetPtr< USkeletalMesh > GetWeaponMesh() const override { return ThrowData->WeaponMesh; }
	virtual float GetBaseDamage() const override { return ThrowData->BaseDamage; }
	virtual float GetRangeOfUse() const override { return ThrowData->RangeOfUse; }
	virtual float GetDelayBeforeUse() const override { return ThrowData->DelayBeforeUse; }
	virtual TEnumAsByte<EEquipmentSlot> GetEquipmentSlot() const override { return ThrowData->EquipmentSlot; }

private:

	FThrowWeaponData* ThrowData;
};