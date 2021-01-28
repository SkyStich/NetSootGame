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
}

void ACommandPlayerController::BeginPlay()
{
    Super::BeginPlay();
}