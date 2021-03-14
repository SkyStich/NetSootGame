// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "../NetWorkShooterGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);

	MaxHealth = 100;
	MaxArmor = 120;

	PointDamageResist = 0.2f;
	RadialDamageResist = 0.3f;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwnerRole() == ROLE_Authority)
	{
		GetOwner()->OnTakeRadialDamage.AddDynamic(this, &UHealthComponent::OnPlayerTakeRadialDamage);
		GetOwner()->OnTakePointDamage.AddDynamic(this, &UHealthComponent::OnPlayerTakePointDamage);
		CurrentHealth = MaxHealth;
		CurrentArmor = MaxArmor;
	}
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/** DOREPLIFETIME with use condition */
	DOREPLIFETIME_CONDITION(UHealthComponent, CurrentHealth, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, CurrentArmor, COND_OwnerOnly);
	
	/** DOREPLIFETIME do not use condition */
	DOREPLIFETIME(UHealthComponent, bIsDead);
}

void UHealthComponent::NetMulticastCharacterDead_Implementation(AController* OldController)
{
	if(bIsDead)
	{
		HealthEndedEvent.Broadcast(OldController);
	}
}

bool UHealthComponent::CheckHealth(AController* InstigatorController, AActor* DamageCauser)
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		if(CurrentHealth <= 0)
		{
			bIsDead = true;
		
			auto const GameMode = Cast<ANetWorkShooterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if(GameMode)
			{
				GetOwner()->SetCanBeDamaged(false);
				NetMulticastCharacterDead(GetOwner()->GetInstigatorController());
				GameMode->CharacterDead(GetOwner()->GetInstigatorController(), InstigatorController, DamageCauser);
				return true;
			}
		}
	}
	return false;
}

float UHealthComponent::ArmorResist(float BaseDamage, float const Resist)
{
	float const NewDamage = BaseDamage * Resist;
	float const TempArmor = CurrentArmor - (BaseDamage - NewDamage);
	CurrentArmor = UKismetMathLibrary::Max(TempArmor, 0);
	return NewDamage + (TempArmor < 0 ? abs(TempArmor) : 0);
}

void UHealthComponent::OnPlayerTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	float const TempDamage = ArmorResist(Damage, PointDamageResist);
	CurrentHealth -= TempDamage;

	Client_NotifyDamage(ShotFromDirection, TempDamage);
	
	CheckHealth(InstigatedBy, DamageCauser);
}

void UHealthComponent::OnPlayerTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	auto const TempDamage = ArmorResist(Damage, RadialDamageResist);
	CurrentHealth -= TempDamage;

	Client_NotifyDamage(Origin, TempDamage);
	CheckHealth(InstigatedBy, DamageCauser);
}

void UHealthComponent::Client_NotifyDamage_Implementation(const FVector& DamageVector, float Damage)
{
	OnClientOwnerReceiveDamageEvent.Broadcast(DamageVector, Damage);
}

