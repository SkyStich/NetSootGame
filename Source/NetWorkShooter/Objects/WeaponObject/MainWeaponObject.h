// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NetWorkShooter/DataAssets/WeaponDataAssetBase.h"
#include "MainWeaponObject.generated.h"

class ANetWorkShooterCharacter;
struct FRangeWeaponData;

UCLASS(Abstract)
class NETWORKSHOOTER_API UMainWeaponObject : public UObject
{
	GENERATED_BODY()

public:
    
    UMainWeaponObject();

    /** define that object in supported network and cun be replicated */
    virtual bool IsSupportedForNetworking() const override { return true; }
    
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
    virtual bool CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack) override;
    
    /** Call when create class */
    virtual void Init(UDataTable* WeaponData, TCHAR* ContextString) {}
    
    /** Set current weapon name */
    void SetWeaponName(FName const NewName) { WeaponName = NewName; }
    void SetCharacterOwner(ANetWorkShooterCharacter* NewOwner) { CharacterOwner = NewOwner; }

    /** start getting var from base weapon struct */
    UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
    virtual TAssetPtr< USkeletalMesh > GetWeaponMesh() const { return nullptr; }

    UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
    virtual float GetBaseDamage() const { return 0.f; }

    UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
    virtual float GetRangeOfUse() const { return 0.f; }

    UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
    virtual float GetDelayBeforeUse() const { return 0.f; }

    UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
    virtual TEnumAsByte<EEquipmentSlot> GetEquipmentSlot() const { return EEquipmentSlot::None; }
    /** stop getting var from base weapon struct */

    /** Get current weapon name */
    UFUNCTION(BlueprintPure, Category = "Weapon|Getter")
    FName GetWeaponName() const { return WeaponName; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|UseWeapon")
    virtual bool UseWeapon();

    UFUNCTION()
    virtual void StopRateDelay();

    UFUNCTION()
    virtual void StopUseWeapon();
    
    /** Create custom begin play */
    UFUNCTION()
    virtual void BeginPlay();

protected:

    virtual void PostInitProperties() override;

    /** return true, if we can use weapon naw */
    virtual bool IsAbleToUseWeapon();
    
    /** return true if this authority */
    UFUNCTION()
    bool GetAuthority() const;
    
protected:

    /** true - if we use weapon naw */
    bool bUseWeapon;

    /** Timer use for delay before use weapon */
    FTimerHandle UseWeaponHandle;
    
    UPROPERTY(Replicated)
    ANetWorkShooterCharacter* CharacterOwner;

private:

    UPROPERTY(Replicated)
    FName WeaponName;
};
