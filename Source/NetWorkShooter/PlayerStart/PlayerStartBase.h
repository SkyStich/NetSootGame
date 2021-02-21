// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"
#include "NetWorkShooter/NetWorkShooterGameMode.h"
#include "NetWorkShooter/GameModes/CommandGameMode.h"

#include "PlayerStartBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerSpawned, ANetWorkShooterCharacter*, SpawnedPlayer);

UCLASS()
class NETWORKSHOOTER_API APlayerStartBase : public APlayerStart
{
	GENERATED_BODY()
	
public:

	void SpawnCharacter(AController* Controller, ANetWorkShooterCharacter* & SpawnedCharacter);
	TEnumAsByte<ETeamList> GetTeam() const { return Team; }

	/** return true if points is free */
	bool CheckOnFreePoints() const;
	
protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable)
	FPlayerSpawned PlayerSpawnedEvent;

private:

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETeamList>Team;
};
