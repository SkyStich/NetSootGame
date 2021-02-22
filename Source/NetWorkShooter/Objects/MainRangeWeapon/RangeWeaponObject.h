// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetWorkShooter/Objects/WeaponObject/MainWeaponObject.h"
#include "RangeWeaponObject.generated.h"

UCLASS(Blueprintable, Abstract)
class NETWORKSHOOTER_API URangeWeaponObject : public UMainWeaponObject
{
	GENERATED_BODY()

	UFUNCTION(NetMulticast, Unreliable)
	void GetTraceInfoDebugger(FVector Start, FVector End, FVector Center);
	void GetTraceInfoDebugger_Implementation(FVector Start, FVector End, FVector Center);

public:

	URangeWeaponObject();

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	FORCEINLINE bool GetIsReloading() const { return bReloading; }
	
	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	FORCEINLINE int32 GetCurrentAmmoInStorage() const { return CurrentAmmoInStorage; }
	
	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	FORCEINLINE int32 GetCurrentAmmoInWeapon() const { return CurrentAmmoInClip; }

	virtual bool UseWeapon() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual TAssetPtr< USkeletalMesh > GetWeaponMesh() const override { return RangeWeaponData->WeaponMesh; }
    virtual float GetBaseDamage() const override { return RangeWeaponData->BaseDamage; }
    virtual float GetRangeOfUse() const override { return RangeWeaponData->RangeOfUse; }
    virtual float GetDelayBeforeUse() const override { return RangeWeaponData->DelayBeforeUse; }
    virtual TEnumAsByte<EEquipmentSlot> GetEquipmentSlot() const override { return RangeWeaponData->EquipmentSlot; }
	
	virtual void Init(UDataTable* WeaponData, TCHAR* ContextString) override;

	void ReloadStart();
	void ReloadFinish();

protected:

	virtual bool IsAbleToUseWeapon() override;
	virtual void BeginPlay() override;
	virtual void StopRateDelay() override;
	
	UFUNCTION()
	FHitResult GetTraceInfo();

private:

	UPROPERTY(Replicated)
	int32 CurrentAmmoInClip;

	UPROPERTY(Replicated)
	int32 CurrentAmmoInStorage;

	UPROPERTY(Replicated)
	bool bReloading;

	UPROPERTY()
	FTimerHandle ReloadHandle;

	FRangeWeaponData* RangeWeaponData;
};
