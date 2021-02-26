// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UChatComponent::UChatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UChatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UChatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UChatComponent::PostMessage(const FString& Message, APlayerState* Player)
{
	NetMulticastMessagePosted(Message, Player);
}

void UChatComponent::NetMulticastMessagePosted_Implementation(const FString& Message, APlayerState* Player)
{
	OnMessagePostedEvent.Broadcast(Message, Player);
}

