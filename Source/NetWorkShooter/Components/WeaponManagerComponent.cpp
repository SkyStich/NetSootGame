// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManagerComponent.h"
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
	bWeaponSelecting = false;

	ConstructorHelpers::FObjectFinder<UWeaponDataAssetBase> WeaponDataHelpers(TEXT("/Game/ThirdPersonCPP/Blueprints/DataAssets/WeaponDataAsset.WeaponDataAsset"));
	if(WeaponDataHelpers.Succeeded())
	{
		WeaponDataAssetBase = WeaponDataHelpers.Object;
	}
}

void UWeaponManagerComponent::OnRep_CurrentWeapon()
{
	OnCurrentWeaponChangedEvent.Broadcast(CurrentWeapon, OldWeapon);
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
	DOREPLIFETIME_CONDITION(UWeaponManagerComponent, Weapons, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UWeaponManagerComponent, OldWeapon, COND_OwnerOnly);
	DOREPLIFETIME(UWeaponManagerComponent, bWeaponSelecting);
	
	/** Replicated without using condition */
	DOREPLIFETIME(UWeaponManagerComponent, CurrentWeapon);
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

UMainWeaponObject* UWeaponManagerComponent::GetWeaponByCategory(TEnumAsByte<EEquipmentSlot> const Category)
{
	for(auto& ByArray : Weapons)
	{
		if(ByArray.Key == Category)
		{
			return ByArray.Value;
		}
	}
	return nullptr;
}

UMainWeaponObject* UWeaponManagerComponent::CreateWeaponObject(FName const WeaponName, TEnumAsByte<EGlobalItemData> GlobalCategory)
{
	/** If this function called in server */
	if(GetOwnerRole() == ROLE_Authority)
	{
		/** Create and init temp variable */
		UMainWeaponObject* NewWeapon = WeaponDataAssetBase->CreateWeaponObject(GetWorld(), WeaponName, GlobalCategory, CharacterOwner);
		
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
		OldWeapon = CurrentWeapon;
		CurrentWeapon = NewCurrentWeapon;
		OnRep_CurrentWeapon();
	}
}

bool UWeaponManagerComponent::Client_WeaponSelect(TEnumAsByte<EEquipmentSlot> const NewSlot)
{
	if(CurrentWeapon->GetEquipmentSlot() != NewSlot && !CurrentWeapon->GetUseWeapon())
	{
		Server_SelectWeapon(NewSlot);
		return true;
	}
	return false;
}

void UWeaponManagerComponent::Server_SelectWeapon_Implementation(EEquipmentSlot NewActiveSlot)
{
	SelectWeapon(NewActiveSlot);
}
		
void UWeaponManagerComponent::OnRep_SelectWeapon()
{
	OnSelectWeaponEvent.Broadcast(bWeaponSelecting);
}

void UWeaponManagerComponent::SelectWeapon(TEnumAsByte<EEquipmentSlot> const NewActiveSlot)
{
	if(CurrentWeapon->GetEquipmentSlot() != NewActiveSlot)
	{
		auto const TempNewWeapon = Weapons.Find(NewActiveSlot);
		if(TempNewWeapon && !CurrentWeapon->GetUseWeapon())
		{
			FTimerDelegate TimerDel;
			FTimerHandle SelectWeaponHandle;
			TimerDel.BindUObject(this, &UWeaponManagerComponent::HalfSelectCompleted, *TempNewWeapon);
			GetWorld()->GetTimerManager().SetTimer(SelectWeaponHandle, TimerDel, 0.4f, false);
			
			bWeaponSelecting = true;
            OnRep_SelectWeapon();
		}
	}
}

void UWeaponManagerComponent::HalfSelectCompleted(UMainWeaponObject* NewWeapon)
{
	FTimerHandle SelectWeaponHandle;
	GetWorld()->GetTimerManager().SetTimer(SelectWeaponHandle, this, &UWeaponManagerComponent::SelectCompleted, 0.6f, false);
	SetCurrentWeapon(NewWeapon);
}

void UWeaponManagerComponent::SelectCompleted()
{
	bWeaponSelecting = false;
	OnRep_SelectWeapon();
}

void UWeaponManagerComponent::AddWeapon(UMainWeaponObject* WeaponToAdd)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		/** Set owner's for new weapon and add her in storage */
		WeaponToAdd->SetCharacterOwner(CharacterOwner);
		Weapons.Add(WeaponToAdd->GetEquipmentSlot(), WeaponToAdd);	
	}
}

void UWeaponManagerComponent::RemoveWeapon(TEnumAsByte<EEquipmentSlot> const SlotForRemove)
{
	if(SlotForRemove != EEquipmentSlot::Melee && SlotForRemove != EEquipmentSlot::SecondWeapon)
	{
		Weapons.Remove(SlotForRemove);	
		Client_WeaponRemove(SlotForRemove);
	}
}

void UWeaponManagerComponent::Client_WeaponRemove_Implementation(EEquipmentSlot RemoveSlot)
{
	OnWeaponRemovedEvent.Broadcast(RemoveSlot);
}

void UWeaponManagerComponent::DropWeaponToWorld(UMainWeaponObject* WeaponToDrop)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		if(WeaponToDrop->GetEquipmentSlot() == EEquipmentSlot::MainWeapon || WeaponToDrop->GetEquipmentSlot()  == EEquipmentSlot::Disposable)
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
				SpawnWeapon->WeaponMesh = WeaponDataAssetBase->GetWeaponMesh(WeaponToDrop->GetWeaponMesh());
				RemoveWeapon(WeaponToDrop->GetEquipmentSlot());
			}
		}	
	}
}