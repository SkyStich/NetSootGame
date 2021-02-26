// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpecialWeapon.h"
#include "NetWorkShooter/DataAssets/WeaponDataAssetBase.h"

// Sets default values
ABaseSpecialWeapon::ABaseSpecialWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;
}

// Called when the game starts or when spawned
void ABaseSpecialWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseSpecialWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseSpecialWeapon::Detonation_Implementation(float BaseDamage, float const MaxDamageRadius, AController* InstigatorController)
{

}

