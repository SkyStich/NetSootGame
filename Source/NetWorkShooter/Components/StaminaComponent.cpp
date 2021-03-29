// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);
	
	CurrentStamina = MaxStamina = 120.f;
	ChangeStaminaPerSec = 20.f;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UStaminaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UStaminaComponent, CurrentStamina, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UStaminaComponent, bStaminaUsed, COND_SkipOwner);
}

void UStaminaComponent::OnRep_StaminaUsed()
{
	OnStaminaUsedEvent.Broadcast(bStaminaUsed);	
}

void UStaminaComponent::StartUseStamina()
{
	if(CurrentStamina > 0)
	{
		bStaminaUsed = true;
		OnRep_StaminaUsed();
		
		GetWorld()->GetTimerManager().ClearTimer(StaminaUseHandle);
		GetWorld()->GetTimerManager().SetTimer(StaminaUseHandle, this, &UStaminaComponent::IncrementStamina, 0.05f, true);
	}
}

void UStaminaComponent::StopUseStamina()
{
	if(bStaminaUsed)
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaUseHandle);
		GetWorld()->GetTimerManager().SetTimer(StaminaUseHandle, this, &UStaminaComponent::DecrementStamina, 0.05f, true, 2.f);
	}
	
	bStaminaUsed = false;
	OnRep_StaminaUsed();
}

void UStaminaComponent::DecrementStamina()
{
	CurrentStamina += ChangeStaminaPerSec / 10;

	if(CurrentStamina >= MaxStamina)
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaUseHandle);
		CurrentStamina = MaxStamina;
	}
}

void UStaminaComponent::IncrementStamina()
{
	CurrentStamina -= ChangeStaminaPerSec / 15;

	if(CurrentStamina <= 0)
	{
		StopUseStamina();
		OnStaminaEndedEvent.Broadcast();
	}
}

void UStaminaComponent::Server_StartUseStamina_Implementation()
{
	StartUseStamina();
}

void UStaminaComponent::Server_StopUseStamina_Implementation()
{
	StopUseStamina();
}

void UStaminaComponent::ClientStartUseStamina()
{
	Server_StartUseStamina();
	StartUseStamina();
}

void UStaminaComponent::ClientStopUseStamina()
{
	Server_StopUseStamina();
	StopUseStamina();
}



