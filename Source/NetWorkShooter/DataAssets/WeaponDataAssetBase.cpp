// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDataAssetBase.h"
#include "../Objects/WeaponObject/MainWeaponObject.h"
#include "Engine/StreamableManager.h"
#include "NetWorkShooter/SingletonClass/GameSingletonClass.h"

UWeaponDataAssetBase::UWeaponDataAssetBase()
{
   
}

TCHAR* UWeaponDataAssetBase::GlobalDataToString(EGlobalItemData GlobalCategory) const
{
    switch (GlobalCategory)
    {
        case EGlobalItemData::MeleeData:
            return TEXT("MeleeData");
        
        case EGlobalItemData::RangeData:
            return TEXT("RangeData");
        
        case EGlobalItemData::ThrowData:
            return TEXT("ThrowData");
    }
    ensure(false);
    return TEXT("Unknown");
}

/** Synch load assets*/
USkeletalMesh* UWeaponDataAssetBase::GetWeaponMesh(TAssetPtr< USkeletalMesh > MeshPtr) 
{
    if(MeshPtr.IsPending())
    {
        auto const AssetSoftObject = MeshPtr.ToSoftObjectPath();
        
        /** Synchrone load the asset */
        MeshPtr = Cast<USkeletalMesh>(UGameSingletonClass::Get().AssetLoader.LoadSynchronous(AssetSoftObject));
    }
    return MeshPtr.Get();
}

/** Find Weapon object class in asset data */
TSoftClassPtr<class UMainWeaponObject> UWeaponDataAssetBase::GetWeaponObjectClass(FName const AssetName, TEnumAsByte<EGlobalItemData> GlobalCategory) const
{
    FString const ContextString = GlobalDataToString(GlobalCategory);
    return WeaponData.FindRef(GlobalCategory)->FindRow<FBaseWeaponData>(AssetName, ContextString)->WeaponClass;
}

UDataTable* UWeaponDataAssetBase::GetDataTableByGlobalCategory(TEnumAsByte<EGlobalItemData> GlobalCategory) const
{
    return WeaponData.FindRef(GlobalCategory);
}

/** Synchron load and create object*/
UMainWeaponObject* UWeaponDataAssetBase::CreateWeaponObject(UObject* WorldContext, FName const AssetName, TEnumAsByte<EGlobalItemData> GlobalCategory, UObject* Outer)
{
    /** Find Weapon object class in asset data */
    auto const TempWeapon = GetWeaponObjectClass(AssetName, GlobalCategory);
    if(TempWeapon.IsNull())
    {
        FString const InstigatorName = WorldContext != nullptr ? WorldContext->GetFullName() : TEXT("Unknown");
        UE_LOG(LogAssetData, Warning, TEXT("UMyAssetLibrary::SyncCreateObject -- Asset ptr is null for: %s"), *InstigatorName);
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
        Weapon->SetWeaponName(AssetName);
        Weapon->Init(GetDataTableByGlobalCategory(GlobalCategory), GlobalDataToString(GlobalCategory));
    }
    return Weapon;
}

