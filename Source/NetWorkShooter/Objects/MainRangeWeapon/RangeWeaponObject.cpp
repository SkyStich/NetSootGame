// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponObject.h"
#include "Net/UnrealNetwork.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

URangeWeaponObject::URangeWeaponObject()
{

}

void URangeWeaponObject::BeginPlay()
{
    Super::BeginPlay();

    if(GetAuthority())
    {
        CurrentAmmoInClip = WeaponData->MaxAmmoInWeapon;
        CurrentAmmoInStorage = WeaponData->MaxAmmoInStorage;
    }
}

void URangeWeaponObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    /** Condition ammo replicated */
    DOREPLIFETIME_CONDITION(URangeWeaponObject, CurrentAmmoInClip, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(URangeWeaponObject, CurrentAmmoInStorage, COND_OwnerOnly)

    /** Reload replication */
    DOREPLIFETIME(URangeWeaponObject, bReloading);
}

bool URangeWeaponObject::UseWeapon()
{
    if(Super::UseWeapon())
    {
        CurrentAmmoInClip--;
        FHitResult const OutHit = GetTraceInfo();
        if(OutHit.GetActor())
        {
            FVector const UnitVector = UKismetMathLibrary::GetDirectionUnitVector(OutHit.TraceEnd, OutHit.TraceStart);
            UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), WeaponData->BaseDamage, UnitVector, OutHit,
                CharacterOwner->GetController(), CharacterOwner, UDamageType::StaticClass());
        }
        
        FString DebugMessage = FString::Printf(TEXT("CurrentInClip: %d | CurrentInStorage: %d"), CurrentAmmoInClip, CurrentAmmoInStorage);
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, DebugMessage);
    }
    return true;
}

bool URangeWeaponObject::IsAbleToUseWeapon()
{
    return Super::IsAbleToUseWeapon() && CurrentAmmoInClip > 0 && !bReloading;
}

void URangeWeaponObject::StopRateDelay()
{
    Super::StopRateDelay();
    if(WeaponData->CanAutoFire && bUseWeapon)
    {
        UseWeapon();
    }
}

FHitResult URangeWeaponObject::GetTraceInfo()
{
    FHitResult OutHit;
    FVector const ForwardVector = UKismetMathLibrary::GetForwardVector(CharacterOwner->GetControlRotation());
    FVector Start = CharacterOwner->GetWeaponSkeletalMeshComponent()->GetSocketLocation("Muzzle");
    FVector const TraceEnd = ForwardVector * WeaponData->RangeOfUse + Start;

    /** Init Collision Quary param */
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(CharacterOwner);

    /** Init collision object param */
    FCollisionObjectQueryParams CollisionObjectQueryParams;
    CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
    CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

    /** Drop line trace end call multicast rpc function */
    GetWorld()->LineTraceSingleByObjectType(OutHit, Start, TraceEnd, CollisionObjectQueryParams, Params);
    GetTraceInfoDebugger(Start, OutHit.TraceEnd, OutHit.Location);
    return OutHit;
}

void URangeWeaponObject::GetTraceInfoDebugger_Implementation(FVector Start, FVector End, FVector Center)
{
    DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 1.f);
    DrawDebugSphere(GetWorld(), Center, 35, 8, FColor::Purple, false, 1.f);
}

void URangeWeaponObject::ReloadStart()
{
    /** if current ammo in clip < max ammo in clip and we have ammo in storage */
    if(CurrentAmmoInClip < WeaponData->MaxAmmoInWeapon && CurrentAmmoInStorage > 0)
    {
        if(!bReloading)
        {
            if(GetAuthority())
            {
                GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &URangeWeaponObject::ReloadFinish,  WeaponData->ReloadTime, false);
                bReloading = true;
            }
        }
    }
}

void URangeWeaponObject::ReloadFinish()
{
    GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
    bReloading = false;
    
    int32 const NeedAmmoFromMaxClip = WeaponData->MaxAmmoInWeapon - CurrentAmmoInClip;
    if(CurrentAmmoInStorage >= NeedAmmoFromMaxClip)
    {
        CurrentAmmoInStorage -= NeedAmmoFromMaxClip;
        CurrentAmmoInClip += NeedAmmoFromMaxClip;
    }
    else
    {
        CurrentAmmoInClip += CurrentAmmoInStorage;
        CurrentAmmoInStorage = 0;
    }
}
