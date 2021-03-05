// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "BaseSpecialWeapon.h"
#include "BaseMineSpecialWeapon.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class NETWORKSHOOTER_API ABaseMineSpecialWeapon : public ABaseSpecialWeapon
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
    void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,  int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:

	ABaseMineSpecialWeapon();
	
	void Init(struct FThrowWeaponData* SpecialWeaponData);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComponent;

private:

	float BaseDamage;
	float Radius;
};
