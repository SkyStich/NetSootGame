// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGrenadeObject.h"
#include "Kismet/KismetMathLibrary.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"

void UMainGrenadeObject::GrenadeThrow()
{
	FVector const Start = CharacterOwner->GetWeaponSkeletalMeshComponent()->GetSocketLocation("Muzzle");
	FRotator const Rotation = UKismetMathLibrary::ComposeRotators(CharacterOwner->GetController()->GetControlRotation(), FRotator(0.f, 15.f, 0.f));

	FActorSpawnParameters SpawnParam;
	SpawnParam.Instigator = CharacterOwner;
	SpawnParam.Owner = CharacterOwner;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(Start, Rotation, SpawnParam);
}	