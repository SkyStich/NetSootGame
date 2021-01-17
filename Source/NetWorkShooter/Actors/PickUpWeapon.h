// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/Interaction.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PickUpWeapon.generated.h"

UCLASS()
class NETWORKSHOOTER_API APickUpWeapon : public AActor, public IInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	bool InteractWithActor(ANetWorkShooterCharacter* OwnerInteraction);
	virtual	bool InteractWithActor_Implementation(ANetWorkShooterCharacter* OwnerInteraction) override;

	UFUNCTION(BlueprintCallable)
	void SetOwnerObject(UMainWeaponObject* NewOwnerObject) { OwnerObject = NewOwnerObject; }

	UFUNCTION()
	void OnRep_WeaponMesh() { SkeletalMesh->SetSkeletalMesh(WeaponMesh); } 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	UMainWeaponObject* OwnerObject;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh;

public:

	UPROPERTY(ReplicatedUsing = OnRep_WeaponMesh)
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleCollision;
};
