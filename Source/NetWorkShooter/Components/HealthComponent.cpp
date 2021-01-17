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
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwnerRole() == ROLE_Authority)
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnPlayerTakeAnyDamage);
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

void UHealthComponent::OnRep_IsDead()
{
	if(bIsDead)
	{
		HealthEndedEvent.Broadcast();
	}
}

void UHealthComponent::OnPlayerTakeAnyDamage(AActor* DamageActor, float BaseDamage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{ 
	float const NewDamage = (1 - (CurrentArmor / (MaxArmor * 2))) * BaseDamage;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("BaseDamage: %f   NewDamage: %f"), BaseDamage, NewDamage));
	CurrentHealth -= NewDamage;
	CurrentArmor = UKismetMathLibrary::Max(CurrentArmor - BaseDamage, 0);
	
	if(CurrentHealth <= 0)
	{
		bIsDead = true;
		
		auto const GameMode = Cast<ANetWorkShooterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if(GameMode)
		{
			GameMode->CharacterDead(GetOwner()->GetInstigatorController(), InstigatorController, DamageCauser);
			GetOwner()->SetCanBeDamaged(false);
			HealthEndedEvent.Broadcast();
		}
	}
}

