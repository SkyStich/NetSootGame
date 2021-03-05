// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "BaseSpecialWeapon.generated.h"

UCLASS(Abstract)
class NETWORKSHOOTER_API ABaseSpecialWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSpecialWeapon();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SpecialWeapon")
    void Detonation(float BaseDamage, float const MaxDamageRadius, AController* InstigatorController);

public:

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;
};
