// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveSpectatorToKiller.h"

AMoveSpectatorToKiller::AMoveSpectatorToKiller()
{
    PrimaryActorTick.bCanEverTick = false;
    
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->bInheritRoll = false;
    SpringArm->TargetArmLength = 300.f;
    RootComponent = SpringArm;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(RootComponent);
    
    SetActorEnableCollision(false);
}

void AMoveSpectatorToKiller::BeginPlay()
{
    Super::BeginPlay();
}

void AMoveSpectatorToKiller::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

   // SetActorLocation(GetInstigator()->GetActorLocation());
}