// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerState.h"
#include "NetWorkShooter/GameModes/CommandGameMode.h"
#include "CommandPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKSHOOTER_API ACommandPlayerState : public ABasePlayerState
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintPure, Category = "CommandController|Getting")
	TEnumAsByte<ETeamList> GetTeam() const { return Team; }

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
protected:

	UPROPERTY(Replicated)
	TEnumAsByte<ETeamList> Team;
};
