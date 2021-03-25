// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaminaComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/WeaponManagerComponent.h"
#include "Components/HealthComponent.h"
#include "NetWorkShooterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FControllerLeave, AController*, OldController);

UCLASS(config=Game)
class ANetWorkShooterCharacter : public ACharacter
{
	GENERATED_BODY()

//	void UseWeaponPressed();
//	void UseWeaponReleased();

	/** This function checked class weapon from optimisation net */
	void ReloadPressed();

	UFUNCTION()
	void CharacterDead(AController* OldController);

	UFUNCTION(Server, Unreliable)
	void Server_ChangeMovementSpeed(float const NewSpeed);

	UFUNCTION()
	void NewCurrentWeapon(UMainWeaponObject* NewWeapon, UMainWeaponObject* OldWeapon);
	
	UFUNCTION()
	void WeaponSelected(bool NewState);
public:
	
	ANetWorkShooterCharacter();

	UFUNCTION(BlueprintCallable)
	void StartUseStamina();
	
	UFUNCTION(BlueprintCallable)
	void StopUseStamina();

	UFUNCTION(BlueprintPure, Category = "Character|Gettting")
	USkeletalMeshComponent* GetWeaponSkeletalMeshComponent() const { return  WeaponSkeletalMeshComponent; }

	UFUNCTION(BlueprintPure, Category = "Character|Gettting")
	UWeaponManagerComponent* GetWeaponManager() const { return WeaponManagerComponent; }

	UFUNCTION(BlueprintPure, Category = "Character|Gettting")
	UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintPure, Category = "Character|LocalMesh")
	USkeletalMeshComponent* GetLocalMesh();

	UFUNCTION(BlueprintPure, Category = "Charcater|States")
	UStaminaComponent* GetStaminaComponent() const { return StaminaComponent; }

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(BlueprintAssignable)
	FControllerLeave OnControllerLeaveEvent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* FirstPersonMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

private:

	/** Weapon skeletal mesh */
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* WeaponSkeletalMeshComponent;

	/** The manager who is responsible for the player's weapon */
	UPROPERTY(VisibleAnywhere)
	UWeaponManagerComponent* WeaponManagerComponent;
	
	UPROPERTY(VisibleAnywhere)
	UStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere)
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category = "Character|ThirdPersonMesh")
	USkeletalMesh* ThirdPersonMesh;
};

