// Fill out your copyright notice in the Description page of Project Settings.


#include "MainThrowingWeapon.h"
#include "Net/UnrealNetwork.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"

void UMainThrowingWeapon::Init(UDataTable* WeaponData, TCHAR* ContextString)
{
	ThrowData = *WeaponData->FindRow<FThrowWeaponData>(GetWeaponName(), ContextString);
	CurrentAmount = ThrowData.MaxAmount;
}

void UMainThrowingWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UMainThrowingWeapon, CurrentAmount, COND_OwnerOnly);
	DOREPLIFETIME(UMainThrowingWeapon, ThrowData);
}

bool UMainThrowingWeapon::UseWeapon()
{
	if(this->IsAbleToUseWeapon() && GetAuthority())
	{		
		bUseWeapon = true;
		CurrentAmount--;		
		CharacterOwner->GetHealthComponent()->HealthEndedEvent.AddDynamic(this, &UMainThrowingWeapon::OuterDead);
		
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &UMainThrowingWeapon::PreparationForUse);
		GetWorld()->GetTimerManager().SetTimer(PreparationForUseHandle, TimerDel, 1.f, false);
		
		return true;
	}
	return false;
}

void UMainThrowingWeapon::StopUseWeapon()
{
	if(!GetWorld()->GetTimerManager().IsTimerActive(UseWeaponHandle) && !GetWorld()->GetTimerManager().IsTimerActive(PreparationForUseHandle))
	{
		Super::StopUseWeapon();
		
		if(CurrentAmount > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(UseWeaponHandle, this, &UMainThrowingWeapon::StopRateDelay, ThrowData.DelayBeforeUse, false);
		}
	}	
}

void UMainThrowingWeapon::ChangeCurrentWeapon()
{
	if(CurrentAmount <= 0)
	{
		CharacterOwner->GetWeaponManager()->SelectWeapon(MainWeapon);
	}
}

bool UMainThrowingWeapon::IsAbleToUseWeapon()
{
	return Super::IsAbleToUseWeapon() && CurrentAmount > 0 && !GetWorld()->GetTimerManager().IsTimerActive(PreparationForUseHandle);
}

FString UMainThrowingWeapon::GetAmmoStats()
{
	return CurrentAmount > 0 ? "1 / " + FString::FromInt(CurrentAmount - 1) : "0 / 0";
}

