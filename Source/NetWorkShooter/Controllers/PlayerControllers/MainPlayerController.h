// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangedPawn, APawn*, NewPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerShowChat);

/**  Base player controller for all controllers */
UCLASS()
class NETWORKSHOOTER_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	UFUNCTION()
	void PlayerRespawn();

	UFUNCTION()
    void OwnerPressedOpenChat();
	
public:

	AMainPlayerController();

	UFUNCTION(BlueprintAuthorityOnly)
	void SpawnPlayer();

	UFUNCTION(BlueprintAuthorityOnly)
	void LaunchRespawnTimer(float const TimeToRespawn, class ANetWorkShooterGameMode* MainGameMode);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void ServerSendMessage(const FString& Message);

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

	UPROPERTY(BlueprintAssignable)
	FPlayerShowChat OnPlayerPressedOpenChatEvent;
};
