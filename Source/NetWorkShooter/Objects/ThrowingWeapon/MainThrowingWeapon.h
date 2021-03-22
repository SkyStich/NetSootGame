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

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmount() const { return CurrentAmount; }

	virtual void Init(UDataTable* WeaponData, TCHAR* ContextString) override;

	virtual TAssetPtr< USkeletalMesh > GetWeaponMesh() const override { return ThrowData.WeaponMesh; }
	virtual float GetBaseDamage() const override { return ThrowData.BaseDamage; }
	virtual float GetRangeOfUse() const override { return ThrowData.RangeOfUse; }
	virtual float GetDelayBeforeUse() const override { return ThrowData.DelayBeforeUse; }
	virtual USoundCue* GetUseWeaponSound() const { return ThrowData.UseWeaponCue; }
	virtual TEnumAsByte<EEquipmentSlot> GetEquipmentSlot() const override { return ThrowData.EquipmentSlot; }

	virtual bool UseWeapon() override;
	virtual bool IsAbleToUseWeapon() override;
	virtual void StopUseWeapon() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	virtual FString GetAmmoStats() override;

protected:

	virtual void OuterDead(AController* Controller) {}
	virtual void PreparationForUse() {}

	/** Change special weapon if CurrentAmmo <= 0 on MainWeapon */
	void ChangeCurrentWeapon();

protected:

	UPROPERTY(Replicated)
	FThrowWeaponData ThrowData;

	FTimerHandle PreparationForUseHandle;
	
	UPROPERTY(Replicated)
	int32 CurrentAmount;
};
