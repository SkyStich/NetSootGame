// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NetWorkShooter/PlayerState/BasePlayerState.h"
#include "ChatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMessagePosted, const FString&, Message, ABasePlayerState*, Player);

/** Chat component implement in Game state */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKSHOOTER_API UChatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChatComponent();

	UFUNCTION(BlueprintCallable)
	void PostMessage(FString& Message, ABasePlayerState* Player);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastMessagePosted(const FString& Message, ABasePlayerState* Player);

public:

	UPROPERTY(BlueprintAssignable)
	FMessagePosted OnMessagePostedEvent;
};
