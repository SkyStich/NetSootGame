// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WeaponManagerComponent.h"
#include "Components/HealthComponent.h"
#include "NetWorkShooterCharacter.generated.h"

UCLASS(config=Game)
class ANetWorkShooterCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/** Fix Me: ReWrite All server function in him components */
	UFUNCTION()
	void OnRep_CurrentWeaponMesh();

	UFUNCTION(Server, Unreliable)
	void ServerStartUseWeapon();
	void ServerStartUseWeapon_Implementation();

	UFUNCTION(Server, Unreliable)
	void ServerStopUseWeapon();
	void ServerStopUseWeapon_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerReloading();
	void ServerReloading_Implementation();

	/** This function checked class weapon from optimisation net */
	void ReloadPressed();

	UFUNCTION()
	void OnRep_CharacterDead();
	
public:
	
	ANetWorkShooterCharacter();

	UFUNCTION(BlueprintPure, Category = "Character|Gettting")
	USkeletalMeshComponent* GetWeaponSkeletalMeshComponent() const { return  WeaponSkeletalMeshComponent; }

	UFUNCTION(BlueprintPure, Category = "Character|Gettting")
	UWeaponManagerComponent* GetWeaponManager() const { return WeaponManagerComponent; }

	UFUNCTION(BlueprintPure, Category = "Character|Gettting")
	UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	
	UFUNCTION()
	void SetCurrentMesh(USkeletalMesh* NewMesh) { CurrentWeaponMesh = NewMesh; } 

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

private:

	/** Weapon skeletal mesh */
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* WeaponSkeletalMeshComponent;

	/** The manager who is responsible for the player's weapon */
	UPROPERTY(VisibleAnywhere)
	UWeaponManagerComponent* WeaponManagerComponent;

	UPROPERTY(VisibleAnywhere)
	UHealthComponent* HealthComponent;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeaponMesh)
	USkeletalMesh* CurrentWeaponMesh;

	//UPROPERTY(ReplicatedUsing = OnRep_CharacterDead)
//	bool bCharacterDead;
};

