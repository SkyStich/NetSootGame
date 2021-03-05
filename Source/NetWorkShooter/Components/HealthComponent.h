// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthEndedEvent, AController*, OldController);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKSHOOTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	bool CheckHealth(AController* InstigatorController, AActor* DamageCauser);

	/** return damage that will be dealt directly to health */
	float ArmorResist(float BaseDamage, float const Resist);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastCharacterDead(AController* OldController);
	
public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category = "HealthComponent")
	bool GetIsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "HealthComponent")
	float GetMaxHealth() const { return MaxHealth;}

	UFUNCTION(BlueprintPure, Category = "HealthComponent")
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "HealthComponent")
	float GetCurrentArmor() const { return CurrentArmor; }

	UFUNCTION(BlueprintPure, Category = "HealthComponent")
	float GetMaxArmor() const { return MaxArmor; }
protected:

	/** Not exposed to sewn armor. deal damage directly to health */
	UFUNCTION()
    void OnPlayerTakeAnyDamage(AActor* DamageActor, float BaseDamage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser);
	
	UFUNCTION()
	void OnPlayerTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);
	
	UFUNCTION()
	void OnPlayerTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser);
	
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

	/** Responsible for the quality of armor protection. How many times health damage is cut with full armor */
	UPROPERTY(EditAnywhere, Category = "HealthComponent|DamageResist")
	float PointDamageResist;

	UPROPERTY(EditAnywhere, Category = "HealthComponent|DamageResist")
	float RadialDamageResist;

	UPROPERTY(Replicated)
	bool bIsDead;
	
public:

	UPROPERTY(BlueprintAssignable, Category = "HealthManager")
	FHealthEndedEvent HealthEndedEvent;
};