// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStartBase.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../NetWorkShooterCharacter.h"

void APlayerStartBase::BeginPlay()
{
    Super::BeginPlay();

    if(GetLocalRole() == ROLE_Authority)
    {
        GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerStartBase::PlayerLeaveSpawnPoint);
        GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerStartBase::CharacterEnterInSpawnPoint);
    }
}

void APlayerStartBase::CharacterEnterInSpawnPoint(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
    bCharacterInside = false;  
}

void APlayerStartBase::PlayerLeaveSpawnPoint(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
    bCharacterInside = true;
}

void APlayerStartBase::SpawnCharacter(AController* Controller, ANetWorkShooterCharacter* & SpawnedCharacter)
{
    FActorSpawnParameters Param;
    Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Param.Owner = Controller;
    
    auto const SpawnCharacter = GetWorld()->SpawnActor<ANetWorkShooterCharacter>(UGameplayStatics::GetGameMode(GetWorld())->DefaultPawnClass, GetActorTransform(), Param);

    if(SpawnCharacter)
    {
        Controller->Possess(SpawnCharacter);
        SpawnedCharacter = SpawnCharacter;
        PlayerSpawnedEvent.Broadcast(SpawnCharacter);
    }
 
}