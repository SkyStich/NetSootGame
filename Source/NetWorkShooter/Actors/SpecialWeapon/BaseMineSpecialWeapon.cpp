// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMineSpecialWeapon.h"
#include "NetWorkShooter/DataAssets/WeaponDataAssetBase.h"

ABaseMineSpecialWeapon::ABaseMineSpecialWeapon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	SkeletalMesh->SetupAttachment(RootComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->SetupAttachment(SkeletalMesh);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABaseMineSpecialWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseMineSpecialWeapon::BeginOverlap);
	}
}

void ABaseMineSpecialWeapon::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(OtherActor->GetInstigatorController() != GetOwner())
	{
		Detonation(BaseDamage, Radius, Cast<AController>(GetOwner()));
	}
}

void ABaseMineSpecialWeapon::Init(struct FThrowWeaponData* SpecialWeaponData)
{
	BaseDamage = SpecialWeaponData->BaseDamage;
	Radius = SpecialWeaponData->MaxDamageRadius;
}