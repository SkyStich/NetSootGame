// Fill out your copyright notice in the Description page of Project Settings.


#include "MainThrowingWeapon.h"
#include "Net/UnrealNetwork.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"

void UMainThrowingWeapon::Init(UDataTable* WeaponData, TCHAR* ContextString)
{
	ThrowData = WeaponData->FindRow<FThrowWeaponData>(GetWeaponName(), ContextString);
	CurrentAmount = ThrowData->MaxAmount;
}

void UMainThrowingWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UMainThrowingWeapon, CurrentAmount, COND_OwnerOnly);
}

bool UMainThrowingWeapon::UseWeapon()
{
	if(this->IsAbleToUseWeapon() && GetAuthority())
	{
		bUseWeapon = true;
		CurrentAmount--;
		return true;
	}
	return false;
}

void UMainThrowingWeapon::StopUseWeapon()
{
	Super::StopUseWeapon();

	if(!GetWorld()->GetTimerManager().IsTimerActive(UseWeaponHandle))
		GetWorld()->GetTimerManager().SetTimer(UseWeaponHandle, this, &UMainThrowingWeapon::StopRateDelay, ThrowData->DelayBeforeUse, false);

	if(CurrentAmount <= 0)
	{
		CharacterOwner->GetWeaponManager()->SetCurrentWeapon(CharacterOwner->GetWeaponManager()->GetWeaponByCategory(MainWeapon));
	}
}

bool UMainThrowingWeapon::IsAbleToUseWeapon()
{
	return Super::IsAbleToUseWeapon() && CurrentAmount > 0;
}

