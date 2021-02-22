// Fill out your copyright notice in the Description page of Project Settings.


#include "MainThrowingWeapon.h"

void UMainThrowingWeapon::Init(UDataTable* WeaponData, TCHAR* ContextString)
{
	ThrowData = WeaponData->FindRow<FThrowWeaponData>(GetWeaponName(), ContextString);
}