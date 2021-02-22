// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpWeapon.h"
#include "../Objects/WeaponObject/MainWeaponObject.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APickUpWeapon::APickUpWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SkeletalMesh->SetCollisionProfileName("Custom...");
	SkeletalMesh->SetCollisionObjectType(ECC_WorldDynamic);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkeletalMesh->SetCollisionObjectType(ECC_WorldStatic);
	SkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	SkeletalMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	SkeletalMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	SkeletalMesh->SetSimulatePhysics(true);

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BoxCollision"));
	CapsuleCollision->SetCollisionProfileName("Custom...");
	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CapsuleCollision->SetCapsuleSize(14.f, 43.f);
	CapsuleCollision->SetRelativeLocation(FVector(10.f,  0.f,  0.f));
	CapsuleCollision->SetHiddenInGame(false);
	CapsuleCollision->SetupAttachment(SkeletalMesh);
}

// Called when the game starts or when spawned
void APickUpWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickUpWeapon, WeaponMesh);
}

bool APickUpWeapon::InteractWithActor_Implementation(ANetWorkShooterCharacter* OwnerInteraction)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		auto const TempCategory = OwnerObject->GetEquipmentSlot();
		
		/** if interact weapon == Disposable create logic later */
		if(TempCategory == EEquipmentSlot::Disposable)
		{
			return true;
		}
		
		/** Drop and remove weapon to world */
		UMainWeaponObject* OldWeapon;
		OwnerInteraction->GetWeaponManager()->GetWeaponByCategory(TempCategory, OldWeapon);
		OwnerInteraction->GetWeaponManager()->DropWeaponToWorld(OldWeapon);
		OwnerInteraction->GetWeaponManager()->RemoveWeapon(TempCategory);
		
		/** Add weapon in player's storage */
		OwnerInteraction->GetWeaponManager()->AddWeapon(OwnerObject);
		
		/** Set new pick up weapoin in current used */
		OwnerInteraction->GetWeaponManager()->SetCurrentWeapon(OwnerObject);	

		/** Destroy this actor after successful interaction */
		Destroy();
		return true;
	}
	return false;
}
