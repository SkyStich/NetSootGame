// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetWorkShooterGameMode.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Spectators/MoveSpectatorToKiller.h"
#include "NetWorkShooter/GameStates/BaseGameState.h"
#include "NetWorkShooter/PlayerState/BasePlayerState.h"
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

	/** Inut start player and States */
	bStartPlayersAsSpectators = true;
	GameStateClass = ABaseGameState::StaticClass();
	PlayerStateClass = ABasePlayerState::StaticClass();
}

void ANetWorkShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStartBase::StaticClass(),AllStartPoints);

	/** Only test */
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANetWorkShooterGameMode::StartGameMatch, 5, false);
	/** Only test */
}

void ANetWorkShooterGameMode::CharacterDead(AController* LoserController, AController* DeathInstigator, AActor* KillingCauser)
{
	PlayerDeadEvent.Broadcast(LoserController, DeathInstigator, KillingCauser);

	/** UpDate points, kills, deaths... For the killer and the murdered */
	UpDateDeathPoints(LoserController->GetPlayerState<ABasePlayerState>(), DeathInstigator->GetPlayerState<ABasePlayerState>());

	/** Spawn spectator for loser controller */
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

void ANetWorkShooterGameMode::StartGameMatch()  
{
	for(auto& ByArray : GameState->PlayerArray)
	{
		SpawnPlayer(Cast<AController>(ByArray->GetOwner()));
	}
	MatchStartedEvent.Broadcast();
}

void ANetWorkShooterGameMode::UpDateDeathPoints(ABasePlayerState* LoserState, ABasePlayerState* InstigatorState)
{
	if(LoserState != InstigatorState)
	{
		/** Add one death of loser controller */
		LoserState->IncrementNumberOfDeaths();

		/** Add one murber of the instigator controller */
		InstigatorState->IncrementNumberOfMurders();
	}
	else
	{
		/** Add one death and reduce one of murder if we killed ourselves */
		LoserState->DecrementNumberOfMurders();
		LoserState->IncrementNumberOfDeaths();
	}
}