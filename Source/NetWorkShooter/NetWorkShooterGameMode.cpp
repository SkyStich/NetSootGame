// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetWorkShooterGameMode.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Spectators/MoveSpectatorToKiller.h"
#include "NetWorkShooter/PlayerStart/PlayerStartBase.h"
#include "UObject/ConstructorHelpers.h"

ANetWorkShooterGameMode::ANetWorkShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	
}

void ANetWorkShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStartBase::StaticClass(),AllStartPoints);	
	
}

void ANetWorkShooterGameMode::CharacterDead(AController* LoserController, AController* DeathInstigator, AActor* KillingCauser)
{
	PlayerDeadEvent.Broadcast(LoserController, DeathInstigator, KillingCauser);
	
	AMainSpectatorPawn* NewSpectatorPawn;
	SpawnSpectator(LoserController, DeathInstigator, NewSpectatorPawn);
}

void ANetWorkShooterGameMode::GetFreeSpawnPoints(TArray<APlayerStartBase*> & FreePoints)
{
	for(auto& ByArray : AllStartPoints)
	{
		auto const TempPoint = Cast<APlayerStartBase>(ByArray);
		if(!TempPoint->bCharacterInside)
		{
			FreePoints.Add(TempPoint);
		}
	}
}

void ANetWorkShooterGameMode::SpawnPlayer(AController* Controller)
{
	TArray<APlayerStartBase*>FreePoints;
	ANetWorkShooterCharacter* SpawnCharacter;
	GetFreeSpawnPoints(FreePoints);
	FreePoints[UKismetMathLibrary::RandomIntegerInRange(0, FreePoints.Num() - 1)]->SpawnCharacter(Controller, SpawnCharacter);	
}
