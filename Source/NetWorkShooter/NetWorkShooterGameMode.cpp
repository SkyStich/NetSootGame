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
	GetGameState<ABaseGameState>()->OnMatchTimeIsOverEvent.AddDynamic(this, &ANetWorkShooterGameMode::MatchTimeEnded);
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
	/** Get all free points for spawn player in naw time  */
	for(auto& ByArray : AllStartPoints)
	{
		auto const TempPoint = Cast<APlayerStartBase>(ByArray);
		if(!TempPoint->bCharacterInside)
		{
			/** Add free point */
			FreePoints.Add(TempPoint);
		}
	}
}

void ANetWorkShooterGameMode::SpawnPlayer(AController* Controller)
{
	/** Local arrey save all free point where can be spawn the player */
	TArray<APlayerStartBase*>FreePoints;

	/** Get Free spawn points */
	GetFreeSpawnPoints(FreePoints);

	/** Get random one point where will be spawn player */
	ANetWorkShooterCharacter* SpawnCharacter;
	FreePoints[UKismetMathLibrary::RandomIntegerInRange(0, FreePoints.Num() - 1)]->SpawnCharacter(Controller, SpawnCharacter);	
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
		GetOwner()->InputComponent->bBlockInput = true;
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