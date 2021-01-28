// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandPlayerController.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ACommandPlayerController::ACommandPlayerController()
{
    
}

void ACommandPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACommandPlayerController, Team);
}

void ACommandPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if(GetLocalRole() == ROLE_Authority)
    {
        UKismetMathLibrary::RandomBool() ? Team = ETeamList::TeamA : Team = ETeamList::TeamB;
    }
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Team: %d"), Team.GetValue()));
}