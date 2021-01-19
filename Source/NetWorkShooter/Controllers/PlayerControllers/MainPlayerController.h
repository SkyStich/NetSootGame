// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class NETWORKSHOOTER_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	UFUNCTION(Server, Reliable)
	void ServerPlayerRespawn();
	void ServerPlayerRespawn_Implementation();

	UFUNCTION()
	void RespawnKeyReleased();
	
public:

	AMainPlayerController();

	UFUNCTION()
	void SpawnPlayer();

	UFUNCTION()
	void LauchRespawnTimer(float const TimeBeforeRespawn);

protected:

	virtual void SetupInputComponent() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleTabMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void HideTabMenu();

private:

	FTimerHandle RespawnTimer;
};
