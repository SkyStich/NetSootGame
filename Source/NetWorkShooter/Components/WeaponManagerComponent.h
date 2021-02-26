// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NetWorkShooter/DataAssets/WeaponDataAssetBase.h"
#include "WeaponManagerComponent.generated.h"

class ANetWorkShooterCharacter;
class UMainWeaponObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentWeaponChanged, UMainWeaponObject*, NewCurrentWeapon);

/** The manager who is responsible for the player's weapon */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKSHOOTER_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void OnRep_CurrentWeapon();

public:

	/** Server func */
	UFUNCTION(Server, Unreliable)
	void ServerStartUseWeapon();
	void ServerStartUseWeapon_Implementation();

	UFUNCTION(Server, Unreliable)
	void ServerStopUseWeapon();
	void ServerStopUseWeapon_Implementation();
	
	// Sets default values for this component's properties
	UWeaponManagerComponent();

	/** Set the weapon that the player is currently using */
	UFUNCTION(BlueprintCallable, Category = "WeaponManager|Setter")
	void SetCurrentWeapon(UMainWeaponObject* const NewCurrentWeapon);

	UFUNCTION(BlueprintCallable, Category = "WeaponManager|Setter")
	void GetWeaponByCategory(TEnumAsByte<EEquipmentSlot> const Category, UMainWeaponObject* & ReturnValue);
	UMainWeaponObject* GetWeaponByCategory(TEnumAsByte<EEquipmentSlot> const Category);
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	/** Add weapon to storage */
	UFUNCTION(BlueprintCallable)
	void AddWeapon(UMainWeaponObject* WeaponToAdd);

	/** Remove main of weapon. Called if we pick up a new weapon */
	UFUNCTION()
    void RemoveWeapon(TEnumAsByte<EEquipmentSlot> const SlotForRemove);

	/** Drop weapon to world. Called if we pick up a new weapon or die */
	UFUNCTION(BlueprintCallable)
	void DropWeaponToWorld(UMainWeaponObject* WeaponToDrop);

	/** Create weapon before game */
	UFUNCTION(BlueprintCallable, Category = "WeaponManager|CreateWeapon")
	UMainWeaponObject* CreateWeaponObject(FName const WeaponName, TEnumAsByte<EGlobalItemData> GlobalCategory);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Select")
	void SelectWeapon(TEnumAsByte<EEquipmentSlot> const NewActiveSlot);

	/** Get the weapon that the player is currently using */
	UFUNCTION(BlueprintPure, Category = "WeaponManager|Getting")
	UMainWeaponObject* GetCurrentWeapon() const { return CurrentWeapon; }
	
	/** Get weapon list owned by player */
	UFUNCTION(BlueprintPure, Category = "WeaponManager|Getting")
    void GetWeapons(TMap<TEnumAsByte<EEquipmentSlot>, UMainWeaponObject*> & NewWeapons) const {  NewWeapons = Weapons; }
    auto GetWeapons() const { return Weapons; }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/** List weapon owned by player(Replicated Owner only) */
	UPROPERTY(Replicated) 
	TMap<TEnumAsByte<EEquipmentSlot>, UMainWeaponObject*> Weapons;

	/** The weapon that the player is currently using */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	UMainWeaponObject* CurrentWeapon;

	UPROPERTY()
	ANetWorkShooterCharacter* CharacterOwner;

	/** Weapon data asset */
	UPROPERTY()
	UWeaponDataAssetBase* WeaponDataAssetBase;

public:

	UPROPERTY(BlueprintAssignable)
	FCurrentWeaponChanged OnCurrentWeaponChangedEvent;
};
