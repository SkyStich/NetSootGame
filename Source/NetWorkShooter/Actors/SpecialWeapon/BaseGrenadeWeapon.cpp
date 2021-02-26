// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGrenadeWeapon.h"
#include "NetWorkShooter/DataAssets/WeaponDataAssetBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "NetWorkShooter/SingletonClass/GameSingletonClass.h"

ABaseGrenadeWeapon::ABaseGrenadeWeapon()
{
	SkeletalMesh->SetIsReplicated(true);
	ForceImpulse = 9000;
}

void ABaseGrenadeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseGrenadeWeapon::Init(FThrowWeaponData* SpecialWeaponData, float const TimeBeforeDetonation, bool bThrowSucceeded)
{
	if(GetLocalRole() != ROLE_Authority) return;

	/** If the throw was made, the player gives and impulse for the grenade otherwise the grenade falls to the place where the throw should have occurred */
	if(bThrowSucceeded)
	{
		SkeletalMesh->AddImpulse(GetActorForwardVector() * ForceImpulse);
		FVector const AngleImpulse = FVector(UKismetMathLibrary::RandomFloatInRange(-520, 520), UKismetMathLibrary::RandomFloatInRange(-655, 655), UKismetMathLibrary::RandomFloatInRange(-353, 453));
		SkeletalMesh->AddAngularImpulseInRadians(AngleImpulse);
	}
	
	if(TimeBeforeDetonation <= 0)
	{
		Detonation(SpecialWeaponData->BaseDamage, SpecialWeaponData->MaxDamageRadius, Cast<AController>(GetOwner()));
	}
	else
	{
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &ABaseGrenadeWeapon::Detonation, SpecialWeaponData->BaseDamage, SpecialWeaponData->MaxDamageRadius, Cast<AController>(GetOwner()));

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, TimeBeforeDetonation, false);
	}
}