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

	RespawnTime = 5.f;
	
	/** Inut start player and States */
	bStartPlayersAsSpectators = true;
}

void ANetWorkShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	/** Add all points where can be spawned the playert */
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStartBase::StaticClass(),AllStartPoints);

	/** Start match */
	FTimerHandle StartHandle;
	GetWorld()->GetTimerManager().SetTimer(StartHandle, this, &ANetWorkShooterGameMode::StartGameMatch, 3.f, false);

	/** Bind on end match if time exit */
	ABaseGameState* BaseGameState = GetGameState<ABaseGameState>();
	BaseGameState->OnMatchTimeIsOverEvent.AddDynamic(this, &ANetWorkShooterGameMode::MatchTimeEnded);
	BaseGameState->OnExcessDeathsEvent.AddDynamic(this, &ANetWorkShooterGameMode::MatchExcessDeathsEnded);
}

void ANetWorkShooterGameMode::CharacterDead(AController* LoserController, AController* DeathInstigator, AActor* KillingCauser)
{
	PlayerDeadEvent.Broadcast(LoserController, DeathInstigator, KillingCauser);
	
	/** UpDate points, kills, deaths... For the killer and the murdered */
	UpDateDeathPoints(LoserController->GetPlayerState<ABasePlayerState>(), DeathInstigator->GetPlayerState<ABasePlayerState>());

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
	TArray<APlayerStartBase*> TempArray;
	
	/** Get all free points for spawn player in naw time  */
	for(auto ByArray : AllStartPoints)
	{
		auto const TempPoint = Cast<APlayerStartBase>(ByArray);
		if(PointSelectionConditions(SpawnController, TempPoint))
		{
			/** Add free point */
			TempArray.Add(TempPoint);
		}
	}
	FreePoints = TempArray;
}

void ANetWorkShooterGameMode::SpawnPlayer(AController* Controller)
 {
 	/** Local arrey save all free point where can be spawn the player */
 	TArray<APlayerStartBase*>FreePoints;
 
 	/** Get Free spawn points */
 	GetFreeSpawnPoints(FreePoints, Controller);
 
 	/** Get random one point where will be spawn player */
 	ANetWorkShooterCharacter* SpawnCharacter;

	int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, FreePoints.Num() - 1);
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
	MatchStartedEvent.Broadcast();
}

void ANetWorkShooterGameMode::StopGameMatch()
{
	/** Get all  player state and block input him controllers*/
	for(auto& ByArray : GameState->PlayerArray)
	{
	//	GetOwner()->InputComponent->bBlockInput = true;
	}
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

void ANetWorkShooterGameMode::MatchTimeEnded()
{
	StopGameMatch();
	OnMatchStopEvent.Broadcast("Time exit");
}

void ANetWorkShooterGameMode::MatchExcessDeathsEnded()
{
	StopGameMatch();
	OnMatchStopEvent.Broadcast("Excess deaths");
}