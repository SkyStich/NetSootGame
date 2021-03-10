// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetWorkShooter/Objects/WeaponObject/MainWeaponObject.h"
#include "RangeWeaponObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloading);

UCLASS(Blueprintable, Abstract)
class NETWORKSHOOTER_API URangeWeaponObject : public UMainWeaponObject
{
	GENERATED_BODY()

	UFUNCTION(NetMulticast, Reliable)
	void GetTraceInfoDebugger(FVector Start, FVector End, FVector Center);
	void GetTraceInfoDebugger_Implementation(FVector Start, FVector End, FVector Center);

	/** Call in ReloadWeapon */
	UFUNCTION(Server, Unreliable)
	void ServerReloading();

	int32 CalculateDamageWithDistance(const FVector& Start, const FVector& End, float Damage);

public:

	URangeWeaponObject();

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	FORCEINLINE bool GetIsReloading() const { return bReloading; }
	
	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	FORCEINLINE int32 GetCurrentAmmoInStorage() const { return CurrentAmmoInStorage; }
	
	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	FORCEINLINE int32 GetCurrentAmmoInWeapon() const { return CurrentAmmoInClip; }

	/** Call in client if him want reload */
	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();

	/** Stop reload. Reload be not finish succeeded */
	void StopReload() { GetWorld()->GetTimerManager().ClearTimer(ReloadHandle); }

	virtual bool UseWeapon() override;
	virtual void StopUseWeapon() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual TAssetPtr< USkeletalMesh > GetWeaponMesh() const override { return RangeWeaponData.WeaponMesh; }
    virtual float GetBaseDamage() const override { return RangeWeaponData.BaseDamage; }
    virtual float GetRangeOfUse() const override { return RangeWeaponData.RangeOfUse; }
    virtual float GetDelayBeforeUse() const override { return RangeWeaponData.DelayBeforeUse; }
	virtual TEnumAsByte<EEquipmentSlot> GetEquipmentSlot() const override { return RangeWeaponData.EquipmentSlot; }
	
	virtual void Init(UDataTable* WeaponData, TCHAR* ContextString) override;
		
	/** Start process reloading */
	void ReloadStart();

public:

	FReloading OnReloadingEvent;

protected:

	virtual bool IsAbleToUseWeapon() override;
	virtual void StopRateDelay() override;
	virtual bool IsAbleReload();

	UFUNCTION()
	virtual void OnRep_Reloading();
	
	UFUNCTION()
    virtual void DropLineTrace();

	/** spawn emitter and sound on client */
	void PlayerWeaponEffectors();

	/** Finish process reload */
	void ReloadFinish();
	
	FVector GetShootDirection();
	
private:

	UPROPERTY(Replicated)
	int32 CurrentAmmoInClip;

	UPROPERTY(Replicated)
	int32 CurrentAmmoInStorage;

	UPROPERTY(ReplicatedUsing = OnRep_Reloading)
	bool bReloading;

	float CurrentSpread;

	UPROPERTY()
	FTimerHandle ReloadHandle;

	UPROPERTY(Replicated)
	FRangeWeaponData RangeWeaponData;
};
