// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayerController.h"
#include "NetWorkShooter/GameModes/CommandGameMode.h"
#include "CommandPlayerController.generated.h"

UCLASS()
class NETWORKSHOOTER_API ACommandPlayerController : public AMainPlayerController
{
	GENERATED_BODY()

public:
	ACommandPlayerController();

	UFUNCTION(BlueprintPure, Category = "CommandController|Getting")
	TEnumAsByte<ETeamList> GetTeam() const { return Team; }
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(Replicated)
	TEnumAsByte<ETeamList> Team;
	
};
