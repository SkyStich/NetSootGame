// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAssetBase.generated.h"

UENUM(BlueprintType)
enum EEquipmentSlot
{
	None,
	MainWeapon,
	SecondWeapon,
	Melee,
	SpecialSlotOne,
	SpecialSlotTwo,
	SpecialSlotTree,
	Disposable
};

USTRUCT(BlueprintType)
struct FRangeWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	TAssetPtr< USkeletalMesh > WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float RangeOfUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float DelayBeforeUse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float MaxAmmoInStorage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float MaxAmmoInWeapon; //Ammo in clip

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float ReloadTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	bool CanAutoFire; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	TSoftClassPtr< class UMainWeaponObject > WeaponClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	TEnumAsByte<EEquipmentSlot> EquipmentSlot;
};

UCLASS(BlueprintType)
class NETWORKSHOOTER_API UWeaponDataAssetBase : public UDataAsset
{
	GENERATED_BODY()

public:

	UWeaponDataAssetBase();

	UFUNCTION(BlueprintPure, Category = DataAsset)
	FORCEINLINE float GetBaseDamage(FName const AssetName) const { return RangeWeaponData.Find(AssetName)->BaseDamage; }

	UFUNCTION(BlueprintPure, Category = DataAsset)
    FORCEINLINE float GetRangeOfUse(FName const AssetName) const { return RangeWeaponData.Find(AssetName)->RangeOfUse; }
    
	UFUNCTION(BlueprintPure, Category = DataAsset)
    FORCEINLINE float GetDelayBeforeUse(FName const AssetName) const { return RangeWeaponData.Find(AssetName)->DelayBeforeUse; }  
    
	UFUNCTION(BlueprintPure, Category = DataAsset)
   	USkeletalMesh* GetWeaponMesh(FName const AssetName) const;
    
   	UFUNCTION(BlueprintPure, Category = DataAsset)
	TSoftClassPtr<class UMainWeaponObject> GetWeaponObjectClass(FName const AssetName) const;

	UFUNCTION(BlueprintPure, Category = DataAsset)
	float GetReloadTime(FName const AssetName) const { return RangeWeaponData.Find(AssetName)->ReloadTime; }
	
	UFUNCTION(BlueprintPure, Category = DataAsset)
	FORCEINLINE TEnumAsByte<EEquipmentSlot> GetEquipmentSlot(FName const AssetName) const { return RangeWeaponData.Find(AssetName)->EquipmentSlot; }

	FORCEINLINE FRangeWeaponData* GetWeaponData(FName const AssetName) { return RangeWeaponData.Find(AssetName); }
	
	UFUNCTION(BlueprintCallable)
	UMainWeaponObject* CreateWeaponObject(UObject* WorldContext, FName const AssetName, UObject* Outer);

private:

	UPROPERTY(EditAnywhere)
	TMap<FName, FRangeWeaponData> RangeWeaponData;	
};
