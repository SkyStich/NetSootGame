// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerState.h"
#include "Net/UnrealNetwork.h"

ABasePlayerState::ABasePlayerState()
{
    
}

void ABasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

   DOREPLIFETIME(ABasePlayerState, NumberOfMurders);
   DOREPLIFETIME(ABasePlayerState, NumberOfDeaths);
}