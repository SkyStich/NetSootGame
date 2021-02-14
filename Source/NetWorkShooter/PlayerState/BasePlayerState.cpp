// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ABasePlayerState::ABasePlayerState()
{
    bUseCustomPlayerNames = true;
    
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

    SetPlayerName(PlayersName[UKismetMathLibrary::RandomIntegerInRange(0, PlayersName.Num() - 1)]);
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
}

void ABasePlayerState::NetMulticastOwnerDead_Implementation(const FString& LoserName, const FString& InstigatorName, const FString& WeaponName)
{
    OnPlayerDeadEvent.Broadcast(LoserName, InstigatorName, WeaponName);
}