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

void URangeWeaponObject::Init(UDataTable* WeaponData, TCHAR* ContextString)
{
    if(GetAuthority())
    {
        RangeWeaponData = WeaponData->FindRow<FRangeWeaponData>(GetWeaponName(), ContextString);
        SlotCategory = RangeWeaponData->EquipmentSlot;
        CurrentAmmoInClip = RangeWeaponData->MaxAmmoInWeapon;
        CurrentAmmoInStorage = RangeWeaponData->MaxAmmoInStorage;
    }
}

void URangeWeaponObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    /** Condition ammo replicated */
    DOREPLIFETIME_CONDITION(URangeWeaponObject, CurrentAmmoInClip, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(URangeWeaponObject, CurrentAmmoInStorage, COND_OwnerOnly);

    /** Reload replication */
    DOREPLIFETIME(URangeWeaponObject, bReloading);
}

int32 URangeWeaponObject::CalculateDamageWithDistance(const FVector& Start, const FVector& End, float Damage)
{
    int32 const Distance = UKismetMathLibrary::Vector_Distance(Start, End) - RangeWeaponData->MaxDamageDistance;
    int32 const DamageReduction = Distance > 0 ? Distance / (RangeWeaponData->DamageDrop * 100 ) : 0;
    return UKismetMathLibrary::Max(Damage - DamageReduction, Damage / 2);;
}

bool URangeWeaponObject::UseWeapon()
{
    if(Super::UseWeapon())
    {
        CurrentAmmoInClip--;
        FHitResult const OutHit = GetTraceInfo();
        if(OutHit.GetActor())
        {
            /** Damage with calculate on distance */
            int32 const NewDamage = CalculateDamageWithDistance(OutHit.TraceStart, OutHit.ImpactPoint, RangeWeaponData->BaseDamage);

            /** Find Direction Unit Vector */
            FVector const UnitVector = UKismetMathLibrary::GetDirectionUnitVector(OutHit.TraceEnd, OutHit.TraceStart);
            UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), NewDamage, UnitVector, OutHit, CharacterOwner->GetController(),CharacterOwner, UDamageType::StaticClass());

            if(OutHit.GetComponent()->IsSimulatingPhysics())
            {
                OutHit.GetComponent()->AddImpulseAtLocation(CharacterOwner->GetVelocity() * NewDamage, OutHit.ImpactPoint);
            }
        }
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
    if(RangeWeaponData->CanAutoFire && bUseWeapon)
    {
        UseWeapon();
    }
}

FHitResult URangeWeaponObject::GetTraceInfo()
{
    FHitResult OutHit;
    FVector const ForwardVector = UKismetMathLibrary::GetForwardVector(CharacterOwner->GetControlRotation());
    FVector Start = CharacterOwner->GetWeaponSkeletalMeshComponent()->GetSocketLocation("Muzzle");
    FVector const TraceEnd = ForwardVector * RangeWeaponData->RangeOfUse + Start;

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

bool URangeWeaponObject::IsAbleReload()
{
    /** if current ammo in clip < max ammo in clip and we have ammo in storage */
    return CurrentAmmoInClip < RangeWeaponData->MaxAmmoInWeapon && CurrentAmmoInStorage > 0 && !bReloading;
}


void URangeWeaponObject::ServerReloading_Implementation()
{
    if(IsAbleReload())
    {
        ReloadStart();
    }
}

void URangeWeaponObject::ReloadWeapon()
{
    if(IsAbleReload())
    {
        ServerReloading();
    }
}

void URangeWeaponObject::OnRep_Reloading()
{
    if(bReloading)
    {
        OnReloadingEvent.Broadcast();
    }
}

void URangeWeaponObject::ReloadStart()
{
    GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &URangeWeaponObject::ReloadFinish,  RangeWeaponData->ReloadTime, false);
    bReloading = true;
}

void URangeWeaponObject::ReloadFinish()
{
    GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
    bReloading = false;
    
    int32 const NeedAmmoFromMaxClip = RangeWeaponData->MaxAmmoInWeapon - CurrentAmmoInClip;
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
