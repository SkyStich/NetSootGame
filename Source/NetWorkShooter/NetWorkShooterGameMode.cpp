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
#include "EngineUtils.h"

ANetWorkShooterGameMode::ANetWorkShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	RespawnTime = 5.f;
	
	/** Inut start player and States */
	bStartPlayersAsSpectators = true;
}

void ANetWorkShooterGameMode::BeginPlay()
{
	Super::BeginPlay();
}

TArray<APlayerStartBase*> ANetWorkShooterGameMode::GetSpawnPoints()
{
	TArray<APlayerStartBase*> Points;
	for (TActorIterator<APlayerStartBase> It(GetWorld()); It; ++It)
	{
		APlayerStartBase* Start = *It;
		if(Start)
		{
			Points.Add(Start);
		}
	}
	return Points;
}

void ANetWorkShooterGameMode::CharacterDead(AController* LoserController, AController* DeathInstigator, AActor* KillingCauser)
{	
	PlayerDeadEvent.Broadcast(LoserController, DeathInstigator, KillingCauser);
	
	/** UpDate points, kills, deaths... For the killer and the murdered */
	UpDateDeathPoints(LoserController, DeathInstigator);

	/** Spawn spectator for loser controller */
	AMainSpectatorPawn* NewSpectatorPawn;
	SpawnSpectator(LoserController, DeathInstigator, NewSpectatorPawn);

	if(bCanAutoRespawn)
	{
		auto const MainPlayerController = Cast<AMainPlayerController>(LoserController);

		if(MainPlayerController)
		{
			MainPlayerController->LaunchRespawnTimer(RespawnTime, this);
		}
	}
}

void ANetWorkShooterGameMode::GetFreeSpawnPoints(TArray<APlayerStartBase*> & FreePoints, AController* SpawnController)
{
	auto const Points = GetSpawnPoints();
	/** Get all free points for spawn player in naw time  */
	for(auto& ByArray : Points)
	{
		auto const TempPoint = Cast<APlayerStartBase>(ByArray);
		if(TempPoint)
		{
			if(PointSelectionConditions(SpawnController, TempPoint))
			{
				/** Add free point */
				FreePoints.Add(TempPoint);
			}
		}	
	}
}

void ANetWorkShooterGameMode::SpawnPlayer(AController* Controller)
 {
 	/** Local arrey save all free point where can be spawn the player */
 	TArray<APlayerStartBase*>FreePoints;
 
 	/** Get Free spawn points */
 	GetFreeSpawnPoints(FreePoints, Controller);
 
 	/** Get random one point where will be spawn player */
 	ANetWorkShooterCharacter* SpawnCharacter;

	int32 const Index = UKismetMathLibrary::RandomIntegerInRange(0, FreePoints.Num() - 1);
	if(FreePoints.IsValidIndex(Index))
 	FreePoints[Index]->SpawnCharacter(Controller, SpawnCharacter);	
 }

bool ANetWorkShooterGameMode::PointSelectionConditions(AController* SpawnController, APlayerStartBase* PointToCheck)
{
	return PointToCheck->bCharacterInside;
}

void ANetWorkShooterGameMode::StartGameMatch()  
{
	/** Get all player state and spawn player ( At the moment the players are being replaced by a spectator, the match itself has not started ) */
	for(auto& ByArray : GameState->PlayerArray)
	{
		SpawnPlayer(Cast<AController>(ByArray->GetOwner()));
	}
	StartGameTimer();
	MatchStartedEvent.Broadcast();
}

void ANetWorkShooterGameMode::StartGameTimer()
{	
	TimerDelegate.BindLambda([&]() -> void
	{
		auto const BaseState = Cast<ABaseGameState>(GameState);
		if(BaseState)
		{
			BaseState->IncrementPlayTime();
			if(CheckGameTimer(BaseState))
			{
				StopGameMatch("Time ended");
			}
		}
	});
	
	GetWorld()->GetTimerManager().SetTimer(TimeTickHandle, TimerDelegate, 1, true);
}

bool ANetWorkShooterGameMode::CheckGameTimer(class ABaseGameState* BaseGameState)
{
	return BaseGameState->GetCurrentPlayTime().IsZero();
}

void ANetWorkShooterGameMode::StopGameMatch(FString StopReason)
{
	/** Get all  player state and block input him controllers*/

	GetWorld()->GetTimerManager().ClearTimer(TimeTickHandle);
	OnMatchStopEvent.Broadcast(StopReason);
}

bool ANetWorkShooterGameMode::UpDateDeathPoints(AController* LoserController, AController* InstigatorController)
{
	auto const LoserState = Cast<ABasePlayerState>(LoserController->PlayerState);
	
	/** if instigator controller is empty, then the player is to blame for his death (but did not kill himself personally), for example, crashed */	
	if(!InstigatorController)
	{
		/** Add one death and reduce one of murder if we killed ourselves */
		LoserState->IncrementNumberOfDeaths();
		
		/** LosetCharacter kill self */
		return false;
	}

	auto const InstigatorState = Cast<ABasePlayerState>(InstigatorController->PlayerState);

	/** if the player has a killer */
	if(LoserState != InstigatorState)
	{
		/** Add one death of loser controller */
		LoserState->IncrementNumberOfDeaths();

		/** Add one murber of the instigator controller */ 
		InstigatorState->IncrementNumberOfMurders();
		return true;
	}
	
	/**
	 *if the player killed himself (exploded on his own grenade)
	 *Add one death and reduce one of murder if we killed ourselves
	 **/
	LoserState->DecrementNumberOfMurders();
	LoserState->IncrementNumberOfDeaths();

	/** LosetCharacter kill self */
	return false;
}