// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMineWeapon.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "NetWorkShooter/Actors/SpecialWeapon/BaseMineSpecialWeapon.h"
#include "DrawDebugHelpers.h"

void UMainMineWeapon::FindPositionForSettingsMine(AController* OwnerController)
{
	GetWorld()->GetTimerManager().ClearTimer(PreparationForUseHandle);
	
	FVector TraceStart = CharacterOwner->GetCapsuleComponent()->GetComponentLocation();
	FVector TraceEnd = OwnerController->GetControlRotation().Vector() * 80.f + TraceStart;

	bool bTraceSucceeded = false;
	
	FHitResult OutHit;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	bTraceSucceeded = GetWorld()->LineTraceSingleByObjectType(OutHit, TraceStart, TraceEnd, ObjectQueryParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, true, 2.f);

	AActor* HorizontalOverlapActors = OutHit.GetActor();

	TraceStart = bTraceSucceeded ? OutHit.ImpactPoint : OutHit.TraceEnd;
	TraceStart += FVector(0.f, 0.f, 6.f);
	TraceEnd = TraceStart - FVector(0.f, 0.f, 180.f);

	bTraceSucceeded = GetWorld()->LineTraceSingleByObjectType(OutHit, TraceStart, TraceEnd, ObjectQueryParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, true, 2.f);
	DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 8.f, 6, FColor::Yellow, false, 2.f);

	if(bTraceSucceeded)
	{
		ABaseMineSpecialWeapon* const Mine = SettingsMine(OwnerController, OutHit.ImpactPoint);
		if(Mine && HorizontalOverlapActors)
		{
			if(Mine->IsOverlappingActor(HorizontalOverlapActors))
			{
				for(int32 i = 0; i < 15; i++)
				{
					Mine->SetActorLocation(Mine->GetActorLocation() - FVector(10.f, 0.f, 0.f));
					if(!Mine->IsOverlappingActor(HorizontalOverlapActors))
					{
						break;
					}
				}
			}
		}
	}
	else
	{
		CurrentAmount++;
		UE_LOG(LogTemp, Warning, TEXT("Not floor for spawn mine."), *GetName());
	}
	
	StopUseWeapon();
	ChangeCurrentWeapon();
}

ABaseMineSpecialWeapon* UMainMineWeapon::SettingsMine(AController* OwnerController, const FVector& SpawnLocation)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerController;
	SpawnParams.Instigator = CharacterOwner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FRotator const Rotation = CharacterOwner->GetCapsuleComponent()->GetComponentRotation();

	auto const SpawnMime = GetWorld()->SpawnActor<ABaseMineSpecialWeapon>(ThrowData.SpecialActor, SpawnLocation, Rotation, SpawnParams);
	if(SpawnMime)
	{
		SpawnMime->Init(&ThrowData);
		return SpawnMime;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Mine can not spawn!"), *GetName());
	}
	return nullptr;
}

void UMainMineWeapon::PreparationForUse()
{
	Super::PreparationForUse();
	
	FindPositionForSettingsMine(CharacterOwner->Controller);
}

void UMainMineWeapon::OuterDead(AController* Controller)
{
	Super::OuterDead(Controller);
	GetWorld()->GetTimerManager().ClearTimer(PreparationForUseHandle);
}

bool UMainMineWeapon::UseWeapon()
{
	return Super::UseWeapon();
}

void UMainMineWeapon::StopUseWeapon()
{
	if(GetAuthority())
	{
		if(!GetWorld()->GetTimerManager().IsTimerActive(PreparationForUseHandle))
		{
			CharacterOwner->GetHealthComponent()->HealthEndedEvent.RemoveDynamic(this, &UMainMineWeapon::OuterDead);
			Super::StopUseWeapon();
		}
	}
}

bool UMainMineWeapon::IsAbleToUseWeapon()
{
	return Super::IsAbleToUseWeapon();
}