// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManagerComponent.h"

#include "ToolContextInterfaces.h"
#include "UObject/ConstructorHelpers.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "../Actors/PickUpWeapon.h"
#include "DrawDebugHelpers.h"
#include "NetWorkShooter/Objects/WeaponObject/MainWeaponObject.h"

// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);

	ConstructorHelpers::FObjectFinder<UWeaponDataAssetBase> WeaponDataHelpers(TEXT("/Game/ThirdPersonCPP/Blueprints/DataAssets/WeaponDataAsset.WeaponDataAsset"));
	if(WeaponDataHelpers.Succeeded())
	{
		WeaponDataAssetBase = WeaponDataHelpers.Object;
	}
}

// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwnerRole() == ROLE_Authority)
	{
		CharacterOwner = Cast<ANetWorkShooterCharacter>(GetOwner());
	}
	
}

void UWeaponManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/** Replicated with using condition */
	DOREPLIFETIME_CONDITION(UWeaponManagerComponent, CurrentWeapon, COND_OwnerOnly);
	//DOREPLIFETIME_CONDITION(UWeaponManagerComponent, CharacterOwner, COND_OwnerOnly);
	
	/** Replicated without using condition */
	DOREPLIFETIME(UWeaponManagerComponent, Weapons);
}

/** Replicate other subobjects (CurrentWeapon) */
bool UWeaponManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool ParentReturn = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if(CurrentWeapon) ParentReturn = Channel->ReplicateSubobject(CurrentWeapon, *Bunch, *RepFlags);
	
	return ParentReturn;
}

void UWeaponManagerComponent::GetWeaponByCategory(TEnumAsByte<EEquipmentSlot> const Category, UMainWeaponObject* & ReturnValue)
{
	for(auto& ByArray : Weapons)
	{
		if(ByArray.Key == Category)
		{
			ReturnValue = ByArray.Value;
			return;
		}
	}
}

UMainWeaponObject* UWeaponManagerComponent::CreateWeaponObject(FName const WeaponName)
{
	/** If this function called in server */
	if(GetOwnerRole() == ROLE_Authority)
	{
		/** Create and init temp variable */
		UMainWeaponObject* NewWeapon = WeaponDataAssetBase->CreateWeaponObject(GetWorld(), WeaponName, CharacterOwner);
		
		/** Set character owner*/
		NewWeapon->SetCharacterOwner(CharacterOwner);
		AddWeapon(NewWeapon);
		return NewWeapon;
	}
	return nullptr;
}

void UWeaponManagerComponent::SetCurrentWeapon(UMainWeaponObject* const NewCurrentWeapon)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		CurrentWeapon = NewCurrentWeapon;
		CharacterOwner->SetCurrentMesh(WeaponDataAssetBase->GetWeaponMesh(CurrentWeapon->GetWeaponName()));
	}
}

void UWeaponManagerComponent::SelectWeapon(TEnumAsByte<EEquipmentSlot> const NewActiveSlot)
{
	if(WeaponDataAssetBase->GetEquipmentSlot(CurrentWeapon->GetWeaponName()) != NewActiveSlot)
	{
		auto const TempNewWeapon = Weapons.Find(NewActiveSlot);
		if(TempNewWeapon)
		{
			if(GetOwnerRole() == ROLE_Authority)
			{
				CurrentWeapon->StopUseWeapon();
				SetCurrentWeapon(*TempNewWeapon);
			}
			else
			{
				//Play anim...
			}
		}
	}
}

void UWeaponManagerComponent::AddWeapon(UMainWeaponObject* WeaponToAdd)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		/** Set owner's for new weapon and add her in storage */
		WeaponToAdd->SetCharacterOwner(CharacterOwner);
		Weapons.Add(WeaponToAdd->GetWeaponData()->EquipmentSlot, WeaponToAdd);	
	}
}

void UWeaponManagerComponent::RemoveWeapon(TEnumAsByte<EEquipmentSlot> const SlotForRemove)
{
	
	if(SlotForRemove != EEquipmentSlot::Melee && SlotForRemove != EEquipmentSlot::SecondWeapon)
	{
		Weapons.Remove(SlotForRemove);	
	}
}

void UWeaponManagerComponent::DropWeaponToWorld(UMainWeaponObject* WeaponToDrop)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		if(WeaponToDrop->GetWeaponData()->EquipmentSlot == EEquipmentSlot::MainWeapon || WeaponToDrop->GetWeaponData()->EquipmentSlot  == EEquipmentSlot::Disposable)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.Owner = GetOwner();
			SpawnParam.Instigator = CharacterOwner;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FVector const TraceStart = CharacterOwner->GetActorForwardVector() * 15 + CharacterOwner->GetActorLocation();
			
			auto const SpawnWeapon = GetWorld()->SpawnActor<APickUpWeapon>(TraceStart, FRotator(90.f, 0.f, 0.f), SpawnParam);
			if(SpawnWeapon)
			{
				SpawnWeapon->SetOwnerObject(WeaponToDrop);
				SpawnWeapon->WeaponMesh = WeaponDataAssetBase->GetWeaponMesh(WeaponToDrop->GetWeaponName());
				
			}
		}	
	}
}