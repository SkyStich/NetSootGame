// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetWorkShooter/Objects/WeaponObject/MainWeaponObject.h"
#include "RangeWeaponObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReloading, bool, NewState);

UCLASS(Blueprintable, Abstract)
class NETWORKSHOOTER_API URangeWeaponObject : public UMainWeaponObject
{
	GENERATED_BODY()

	UFUNCTION(NetMulticast, Reliable)
	void GetTraceInfoDebugger(FVector Start, FVector End, FVector Center);
	void GetTraceInfoDebugger_Implementation(FVector Start, FVector End, FVector Center);

	int32 CalculateDamageWithDistance(const FVector& Start, const FVector& End, float Damage);
	
	/** Call in ReloadWeapon */
	UFUNCTION(Server, Unreliable)
    void ServerReloading();

	/** Stop reload without calculate ammo */
	UFUNCTION()
	void ClearReload(bool const Clear);

	void ApplyDamageByTrace(const FHitResult& HitResult);

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
	virtual USoundCue* GetUseWeaponSound() const override { return RangeWeaponData.UseWeaponCue; }
	
	virtual void Init(UDataTable* WeaponData, TCHAR* ContextString) override;

	virtual FString GetAmmoStats() override;

protected:

	virtual bool IsAbleToUseWeapon() override;
	virtual void StopRateDelay() override;
	virtual bool IsAbleReload();

	UFUNCTION()
	virtual void OnRep_Reloading();
	
    virtual void DropLineTrace(FHitResult& OutHit);
	virtual void OwnerDead(AController* OldController) override;
	virtual void OnRep_Owner() override;
	virtual void PlayerWeaponEffectors() override;

	/** Start process reloading */
	virtual void ReloadStart();
	
	/** Finish process reload */
	virtual void ReloadFinish();
	
	FVector GetShootDirection();

public:

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Reloading")
	FReloading OnReloadingEvent;
	
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
