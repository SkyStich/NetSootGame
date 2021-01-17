// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetWorkShooter/Objects/WeaponObject/MainWeaponObject.h"
#include "RangeWeaponObject.generated.h"

UCLASS(Blueprintable, Abstract)
class NETWORKSHOOTER_API URangeWeaponObject : public UMainWeaponObject
{
	GENERATED_BODY()

	UFUNCTION(NetMulticast, Reliable)
	void GetTraceInfoDebugger(FVector Start, FVector End, FVector Center);
	void GetTraceInfoDebugger_Implementation(FVector Start, FVector End, FVector Center);

public:

	URangeWeaponObject();

	UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
	bool GetIsReloading() const { return bReloading; }

	virtual bool UseWeapon() override;
	virtual void StopRateDelay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ReloadStart();
	void ReloadFinish();

protected:

	virtual bool IsAbleToUseWeapon() override;
	virtual void BeginPlay() override;

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
	
};
