// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class NETWORKSHOOTER_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	UFUNCTION(Server, Unreliable)
	void ServerPlayerRespawn();
	void ServerPlayerRespawn_Implementation();
	
public:

	AMainPlayerController();

	UFUNCTION()
	void SpawnPlayer();

	UFUNCTION()
	void LaunchRespawnTimer(float const TimeToRespawn, class ANetWorkShooterGameMode* MainGameMode);

protected:

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void MatchEnded(FString Reason);
	
	UFUNCTION()
	void ToggleTabMenu();

	UFUNCTION()
	void HideTabMenu();

private:

	FTimerHandle RespawnTimer;
};
