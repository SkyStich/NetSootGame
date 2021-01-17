// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSpectatorPawn.h"

AMainSpectatorPawn::AMainSpectatorPawn()
{
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;
}

void AMainSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AMainSpectatorPawn::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AMainSpectatorPawn::LookUpAtRate);
}

void AMainSpectatorPawn::BeginPlay()
{
    Super::BeginPlay();

}

void AMainSpectatorPawn::TurnAtRate(float Rate)
{
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainSpectatorPawn::LookUpAtRate(float Rate)
{
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}