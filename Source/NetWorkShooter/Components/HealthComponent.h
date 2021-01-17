// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthEndedEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKSHOOTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void OnRep_IsDead();
	
public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UFUNCTION()
    void OnPlayerTakeAnyDamage(AActor* DamageActor, float BaseDamage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser);
	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(Replicated)
	float CurrentHealth;

	UPROPERTY(EditAnywhere)
	float MaxHealth;

	UPROPERTY(Replicated)
	float CurrentArmor;

	UPROPERTY(EditAnywhere)
	float MaxArmor;

	UPROPERTY(ReplicatedUsing=OnRep_IsDead)
	bool bIsDead;
	
public:

	UPROPERTY(BlueprintAssignable, Category = "HealthManager")
	FHealthEndedEvent HealthEndedEvent;
};