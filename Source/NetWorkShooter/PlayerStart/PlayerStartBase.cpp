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
}

bool APlayerStartBase::CheckOnFreePoints() const
{
    TArray<AActor*>OverlapActors;
    GetCapsuleComponent()->GetOverlappingActors(OverlapActors);

    for(auto& ByArray : OverlapActors)
    {
        auto const TempCharacter = Cast<ANetWorkShooterCharacter>(ByArray);
        if(TempCharacter && !TempCharacter->GetHealthComponent()->GetIsDead())
        {
            return false;
        }
    }
    return true;
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

