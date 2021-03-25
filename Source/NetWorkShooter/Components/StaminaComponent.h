// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaUsed, bool, Used);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaEnded);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKSHOOTER_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

	void StartUseStamina();
	void StopUseStamina();

	void IncrementStamina();
	void DecrementStamina();

	UFUNCTION()
	void OnRep_StaminaUsed();
	
	UFUNCTION(Server, Unreliable)
	void Server_StartUseStamina();

	UFUNCTION(Server, Unreliable)
    void Server_StopUseStamina();

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	UFUNCTION(BlueprintCallable, Category = "Character|Stamina")
	void ClientStopUseStamina();
	
	UFUNCTION(BlueprintCallable, Category = "Character|Stamina")
    void ClientStartUseStamina();
	
	UFUNCTION(BlueprintPure, Category = "Character|Stamina")
	float GetMaxStamina() const { return MaxStamina; }
	
	UFUNCTION(BlueprintPure, Category = "Character|Stamina")
	float GetCurrentStamina() const { return CurrentStamina; }
	
	UFUNCTION(BlueprintPure, Category = "Character|Stamina")
	bool GetIsStaminaUsed() const { return bStaminaUsed; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY(BlueprintAssignable)
	FStaminaUsed OnStaminaUsedEvent;

	UPROPERTY(BlueprintAssignable)
	FStaminaEnded OnStaminaEndedEvent;

private:

	UPROPERTY(Replicated)
	float CurrentStamina;

	UPROPERTY(ReplicatedUsing = OnRep_StaminaUsed)
	bool bStaminaUsed;

	UPROPERTY(EditAnywhere, Category = "Character|State")
	float MaxStamina;
	
	UPROPERTY(EditAnywhere, Category = "Character|State")
	float ChangeStaminaPerSec;

	FTimerHandle StaminaUseHandle;
};
