// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "NetWorkShooter/Actors/Projectiles/MainProjectile/MainProjectile.h"
#include "NetWorkShooter/Actors/SpecialWeapon/BaseSpecialWeapon.h"
#include "Sound/SoundCue.h"

#include "WeaponDataAssetBase.generated.h"

UENUM(BlueprintType)
enum EGlobalItemData
{
	MeleeData,
	RangeData,
	ThrowData
};

UENUM(BlueprintType)
enum EEquipmentSlot
{
	None,
	MainWeapon,
	SecondWeapon,
	Melee,
	SpecialSlot1,
	SpecialSlot2,
	SpecialSlot3,
	Disposable
};

USTRUCT(BlueprintType)
struct FBaseWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	TAssetPtr< USkeletalMesh > WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float RangeOfUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float DelayBeforeUse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	TSoftClassPtr< class UMainWeaponObject > WeaponClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
    USoundCue* UseWeaponCue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	TEnumAsByte<EEquipmentSlot> EquipmentSlot;
};

USTRUCT(BlueprintType)
struct FRangeWeaponData : public FBaseWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float MaxAmmoInStorage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float MaxAmmoInWeapon; //Ammo in clip

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float ReloadTime;
	
	/** At a distance from 0 to MaxDamageDistance the weapon deals max damage (in cm) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	int32 MaxDamageDistance;

	/** Drop in damage for every 100 units passed from (DistanceVector(Start, End) - MaxDamageDistance). Max drop = BaseDamage / 2. 1 unit == 1 damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float DamageDrop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float MaxSpread;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	bool CanAutoFire;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	UParticleSystem* FireParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	TSubclassOf<AMainProjectile>Projectile;
};

USTRUCT(BlueprintType)
struct FThrowWeaponData : public FBaseWeaponData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	int32 MaxAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float TimeBeforeExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	float MaxDamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataAsset)
	TSubclassOf<ABaseSpecialWeapon>SpecialActor;
};

UCLASS(BlueprintType)
class NETWORKSHOOTER_API UWeaponDataAssetBase : public UDataAsset
{
	GENERATED_BODY()

public:

	UWeaponDataAssetBase();

	TCHAR* GlobalDataToString(EGlobalItemData GlobalCategory) const;
    
	UFUNCTION(BlueprintPure, Category = DataAsset)
   	USkeletalMesh* GetWeaponMesh(TAssetPtr< USkeletalMesh > MeshPtr);
    
	TSoftClassPtr<class UMainWeaponObject> GetWeaponObjectClass(FName const AssetName, TEnumAsByte<EGlobalItemData> GlobalCategory) const;
	
	UFUNCTION(BlueprintCallable)
	UMainWeaponObject* CreateWeaponObject(UObject* WorldContext, FName const AssetName, TEnumAsByte<EGlobalItemData> GlobalCategory, UObject* Outer);

	UDataTable* GetDataTableByGlobalCategory(TEnumAsByte<EGlobalItemData> GlobalCategory) const;

private:

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EGlobalItemData>, UDataTable*> WeaponData;
};
