// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NetWorkShooter/DataAssets/WeaponDataAssetBase.h"
#include "MainWeaponObject.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponUsed, UMainWeaponObject*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdditionUsed, bool, bNewAdditionalState);

class ANetWorkShooterCharacter;
struct FRangeWeaponData;

UCLASS(Abstract, Blueprintable)
class NETWORKSHOOTER_API UMainWeaponObject : public UObject
{
	GENERATED_BODY()

    UFUNCTION(Server, Unreliable)
    void Server_UseWeapon();

    UFUNCTION(Server, Unreliable)
    void Server_StopUseWeapon();

    UFUNCTION(NetMulticast, Unreliable)
    void NetMulticast_UseWeapon();
    
    UFUNCTION(NetMulticast, Unreliable)
    void NetMulticast_StopUseWeapon();

    UFUNCTION()
    void OnRep_AdditionalUsed();

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
    
    virtual void SetCharacterOwner(ANetWorkShooterCharacter* NewOwner);
    virtual bool IsAbleToAdditionalUse() {return true; }

    UFUNCTION(BlueprintPure, Category = "Weapon|Additional")
    bool GetAdditionalUsed() const { return bAdditionalUsed; }

    UFUNCTION(BlueprintCallable)
    void Client_UseWeapon();

    UFUNCTION(BlueprintCallable)
    void Client_StopUseWeapon();

    UFUNCTION(BlueprintPure)
    bool GetUseWeapon() const { return bUseWeapon; }

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
    virtual USoundCue* GetUseWeaponSound() const { return nullptr; }

    UFUNCTION(BlueprintPure, Category = "Weapon|Getting")
    virtual TEnumAsByte<EEquipmentSlot> GetEquipmentSlot() const { return EEquipmentSlot::None; }
    /** stop getting var from base weapon struct */

    /** Get current weapon name */
    UFUNCTION(BlueprintPure, Category = "Weapon|Getter")
    FName GetWeaponName() const { return WeaponName; }

    UFUNCTION()
    virtual void StopRateDelay();

    UFUNCTION()
    virtual void StopUseWeapon();
    
    /** Create custom begin play */
    UFUNCTION()
    virtual void BeginPlay();

    UFUNCTION()
    virtual void AdditionalUse();

    UFUNCTION()
    virtual void StopAdditionUse();
    
    UFUNCTION(BlueprintPure)
    virtual FString GetAmmoStats() { return ""; }

protected:

    virtual void PostInitProperties() override;

    /** return true, if we can use weapon naw */
    virtual bool IsAbleToUseWeapon();

    /** spawn emitter and sound on client */
    virtual void PlayerWeaponEffectors();

    UFUNCTION()
    virtual void OwnerDead(AController* OldController);

    UFUNCTION()
    virtual void WeaponSelecting(bool bNewState) {}
    
    /** return true if this authority */
    UFUNCTION()
    bool GetAuthority() const;

    UFUNCTION()
    virtual void OnRep_Owner();
    
    UFUNCTION(BlueprintCallable, Category = "Weapon|UseWeapon")
    virtual bool UseWeapon();

    FORCEINLINE bool IsOtherPlayer() const;

public:

    UPROPERTY(BlueprintAssignable)
    FWeaponUsed OnWeaponUsedEvent;

    UPROPERTY(BlueprintAssignable)
    FAdditionUsed OnAdditionalUsedEvent;
    
protected:

    /** true - if we use weapon naw */
    UPROPERTY(Replicated)
    bool bUseWeapon;

    UPROPERTY(ReplicatedUsing = OnRep_AdditionalUsed)
    bool bAdditionalUsed;

    /** Timer use for delay before use weapon */
    FTimerHandle UseWeaponHandle;
    
    UPROPERTY(ReplicatedUsing = OnRep_Owner)
    ANetWorkShooterCharacter* CharacterOwner;

    UPROPERTY()
    UMaterialInterface* HoleMaterial;

private:

    UPROPERTY(Replicated)
    FName WeaponName;
};
