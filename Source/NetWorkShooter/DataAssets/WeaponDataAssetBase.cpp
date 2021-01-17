// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDataAssetBase.h"
#include "../Objects/WeaponObject/MainWeaponObject.h"
#include "Engine/StreamableManager.h"
#include "NetWorkShooter/SingletonClass/GameSingletonClass.h"

UWeaponDataAssetBase::UWeaponDataAssetBase()
{
   
}

/** Synchron load assets*/
USkeletalMesh* UWeaponDataAssetBase::GetWeaponMesh(FName const AssetName) const
{
    /** Create temp value */
    auto TempMesh = RangeWeaponData.Find(AssetName)->WeaponMesh;

    if(TempMesh.IsPending())
    {
        auto const AssetSoftObject = TempMesh.ToSoftObjectPath();
        
        /** Synchrone load the asset */
        TempMesh = Cast<USkeletalMesh>(UGameSingletonClass::Get().AssetLoader.LoadSynchronous(AssetSoftObject));
    }
    return TempMesh.Get();
}

/** Find Weapon object class in asset data */
TSoftClassPtr<class UMainWeaponObject> UWeaponDataAssetBase::GetWeaponObjectClass(FName const AssetName) const
{
    return RangeWeaponData.Find(AssetName)->WeaponClass;
}

/** Synchron load and create object*/
UMainWeaponObject* UWeaponDataAssetBase::CreateWeaponObject(UObject* WorldContext, FName const AssetName, UObject* Outer)
{
    /** Find Weapon object class in asset data */
    auto const TempWeapon = GetWeaponObjectClass(AssetName);
    if(TempWeapon.IsNull())
    {
        FString const InstigatorName = WorldContext != nullptr ? WorldContext->GetFullName() : TEXT("Unknown");
        UE_LOG(LogAssetData, Warning, TEXT("UMyAssetLibrary::SyncCreateWidget -- Asset ptr is null for: %s"), *InstigatorName);
        return nullptr;
    }

    //load asset into memory (sync)
    FStreamableManager& StreamableManager = UGameSingletonClass::Get().AssetLoader;
    FSoftObjectPath const Reference = TempWeapon.ToSoftObjectPath();
    StreamableManager.LoadSynchronous(TempWeapon);

    //Now load object and check that it has desired class
    UClass* WeaponType = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), Outer, *Reference.ToString()));
    
    /** If the cast did not pass, return nullptr */
    if(WeaponType == nullptr)
    {
        return nullptr;
    }

    /** Create Weapon Object */
    UMainWeaponObject* Weapon = NewObject<UMainWeaponObject>(Outer, WeaponType);
    
    /** Make sure that the object will not be destroyed on this frame */
    if(Weapon)
    {
        Weapon->SetFlags(RF_StrongRefOnFrame);
        Weapon->SetWeaponData(GetWeaponData(AssetName));
        Weapon->SetWeaponName(AssetName);
        Weapon->BeginPlay();
    }
    return Weapon;
}

