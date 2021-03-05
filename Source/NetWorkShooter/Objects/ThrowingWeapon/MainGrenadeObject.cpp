// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGrenadeObject.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerInput.h"
#include "NetWorkShooter/Actors/SpecialWeapon/BaseGrenadeWeapon.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"

void UMainGrenadeObject::BeginPlay()
{
	Super::BeginPlay();
}

void UMainGrenadeObject::GrenadeThrow(float const TotalTimeBeforeExplosion, bool bThrowSucceeded, AController* Controller)
{
	GetWorld()->GetTimerManager().ClearTimer(ExplosionHandle);
	
	StopUseWeapon();
	ChangeCurrentWeapon();
	
	FVector Start = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;

	Start = CharacterOwner->GetWeaponSkeletalMeshComponent()->GetSocketLocation("Muzzle");
	Rotation = UKismetMathLibrary::ComposeRotators(Controller->GetControlRotation(), FRotator(0.f, 0.f, 25.f));

	FActorSpawnParameters SpawnParam;
	SpawnParam.Instigator = CharacterOwner;
	SpawnParam.Owner = Controller;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto const Grenade = GetWorld()->SpawnActor<ABaseGrenadeWeapon>(ThrowData->SpecialActor, Start, Rotation, SpawnParam);
	if(Grenade)
	{
		Grenade->Init(ThrowData, TotalTimeBeforeExplosion, bThrowSucceeded);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Grenade can not spawn."), *GetName());
	}
}

bool UMainGrenadeObject::UseWeapon()
{
	bWantPreparation = true;
	
	return Super::UseWeapon();
}

void UMainGrenadeObject::OuterDead(AController* Controller)
{
	if(GetAuthority())
	{
		GetWorld()->GetTimerManager().ClearTimer(PreparationForUseHandle);
		/** if the timer that is responsible for the delay before the throw is not active and we are currently holding a grenade in our hand*/
		if(GetWorld()->GetTimerManager().IsTimerActive(ExplosionHandle))
		{
			/** We drop the granede in front of its owner's dead and after the remaining time, the grenade will explode */
			GrenadeThrow(GetWorld()->GetTimerManager().GetTimerRemaining(ExplosionHandle), false, Controller);
			GetWorld()->GetTimerManager().ClearTimer(ExplosionHandle);
		}
	}
}

void UMainGrenadeObject::PreparationForUse()
{
	GetWorld()->GetTimerManager().ClearTimer(PreparationForUseHandle);

	if(CharacterOwner->Controller)
	{
		if(bWantPreparation)
		{
			/** if we held the grenade for too long and it exploded in the owner's hand */
			FTimerDelegate TimerDel;
			TimerDel.BindUObject(this, &UMainGrenadeObject::GrenadeThrow, 0.f, true, CharacterOwner->Controller);
			GetWorld()->GetTimerManager().SetTimer(ExplosionHandle, TimerDel, ThrowData->TimeBeforeExplosion, false);
		}
		else
		{
			/** throw the grenade and give it the remaining time before detonating */
			GrenadeThrow(ThrowData->TimeBeforeExplosion, true, CharacterOwner->Controller);
		}
	}
}

void UMainGrenadeObject::StopUseWeapon()
{
	/** We don't want to hold a grenade */
	bWantPreparation = false;

	/** if the timer that is responsible for preparing the grenade for throwing is not active. and the timer before the explosion is still active */
	if(!GetWorld()->GetTimerManager().IsTimerActive(PreparationForUseHandle))
	{
		CharacterOwner->GetHealthComponent()->HealthEndedEvent.RemoveDynamic(this, &UMainGrenadeObject::OuterDead);
		
		if(GetWorld()->GetTimerManager().IsTimerActive(ExplosionHandle))
		{
			/** Throw the grenade and give it the remaining time to throw. To clarify that the owner threw a grenade. */
			GrenadeThrow(GetWorld()->GetTimerManager().GetTimerRemaining(ExplosionHandle), true, CharacterOwner->GetController());

			/** Prevent the object from controlling the timer before detonation */
			GetWorld()->GetTimerManager().ClearTimer(ExplosionHandle);
		}

		/** Call parent logic */
		Super::StopUseWeapon();
	}
}