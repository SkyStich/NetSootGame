// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MainWeaponObject.generated.h"

class ANetWorkShooterCharacter;
class UWeaponDataAssetBase;
struct FRangeWeaponData;

UCLASS(Blueprintable, Abstract)
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

    void SetWeaponData(FRangeWeaponData* NewWeaponData) { WeaponData = NewWeaponData; }

    /** Set current weapon name */
    UFUNCTION(BlueprintCallable, Category = "Weapon|Setter")
    void SetWeaponName(FName const NewName) { WeaponName = NewName; }
    
    UFUNCTION(BlueprintCallable, Category = "Weapon|Setter")
    void SetCharacterOwner(ANetWorkShooterCharacter* NewOwner) { CharacterOwner = NewOwner; }

    /** Get current weapon name */
    UFUNCTION(BlueprintPure, Category = "Weapon|Getter")
    FName GetWeaponName() const { return WeaponName; }

    UFUNCTION(BlueprintCallable, Category = "Weapon|UseWeapon")
    virtual bool UseWeapon();

    UFUNCTION()
    virtual void StopRateDelay();

    UFUNCTION()
    void StopUseWeapon();
    
    /** Create custom begin play */
    UFUNCTION()
    virtual void BeginPlay();

    FRangeWeaponData* GetWeaponData() const { return WeaponData; }

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

    /** Data this weapon */
    FRangeWeaponData* WeaponData;    
    
    UPROPERTY(Replicated)
    ANetWorkShooterCharacter* CharacterOwner;

private:

    UPROPERTY()
    FName WeaponName;
};
