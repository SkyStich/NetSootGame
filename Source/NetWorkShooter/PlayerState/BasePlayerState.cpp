// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerState.h"
#include "Net/UnrealNetwork.h"

ABasePlayerState::ABasePlayerState()
{    
    PlayersName.Add("Jon");
    PlayersName.Add("Peeter");
    PlayersName.Add("Jess");
    PlayersName.Add("Rick");
    PlayersName.Add("Tom");
}

void ABasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

   DOREPLIFETIME(ABasePlayerState, NumberOfMurders);
   DOREPLIFETIME(ABasePlayerState, NumberOfDeaths);
   DOREPLIFETIME(ABasePlayerState, bIsAlive);
}

void ABasePlayerState::BeginPlay()
{
    Super::BeginPlay();

    if(GetLocalRole() == ROLE_Authority)
    {
        SetPlayerName("Jon");
    }
}

void ABasePlayerState::IncrementNumberOfMurders()
{
    NumberOfMurders++;
}

void ABasePlayerState::DecrementNumberOfMurders()
{
    NumberOfMurders--;
}

void ABasePlayerState::IncrementNumberOfDeaths()
{
    NumberOfDeaths++;
}

void ABasePlayerState::SetIsAlive(bool const bNewState)
{
    bIsAlive = bNewState;
    ForceNetUpdate();
}

void ABasePlayerState::NetMulticastOwnerDead_Implementation(const FString& LoserName, const FString& InstigatorName, const FString& WeaponName)
{
    OnPlayerDeadEvent.Broadcast(LoserName, InstigatorName, WeaponName);
}