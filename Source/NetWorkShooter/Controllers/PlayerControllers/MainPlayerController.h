// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangedPawn, APawn*, NewPawn);

UCLASS()
class NETWORKSHOOTER_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	UFUNCTION()
	void PlayerRespawn();
	
public:

	AMainPlayerController();

	UFUNCTION(BlueprintAuthorityOnly)
	void SpawnPlayer();

	UFUNCTION(BlueprintAuthorityOnly)
	void LaunchRespawnTimer(float const TimeToRespawn, class ANetWorkShooterGameMode* MainGameMode);

protected:

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void MatchEnded(FString Reason);
	virtual void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;
	virtual void OnRep_Pawn() override;

	UFUNCTION()
	void ToggleTabMenu();

	UFUNCTION()
	void HideTabMenu();

private:

	FTimerHandle RespawnTimer;


public:

	UPROPERTY(BlueprintAssignable)
	FChangedPawn OnNewPawnEvent;
};
