// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponObject.h"
#include "Net/UnrealNetwork.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

URangeWeaponObject::URangeWeaponObject()
{
    CurrentSpread = 0.f;
}

void URangeWeaponObject::Init(UDataTable* WeaponData, TCHAR* ContextString)
{
    if(GetAuthority())
    {
        FRangeWeaponData* TempRangeData = WeaponData->FindRow<FRangeWeaponData>(GetWeaponName(), ContextString);
        RangeWeaponData = *TempRangeData;
        CurrentAmmoInClip = RangeWeaponData.MaxAmmoInWeapon;
        CurrentAmmoInStorage = RangeWeaponData.MaxAmmoInStorage;
    }
}

void URangeWeaponObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    /** Condition ammo replicated */
    DOREPLIFETIME_CONDITION(URangeWeaponObject, CurrentAmmoInClip, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(URangeWeaponObject, CurrentAmmoInStorage, COND_OwnerOnly);

    /** RangeWeaponData replicated */
    DOREPLIFETIME(URangeWeaponObject, RangeWeaponData);

    /** Reload replication */
    DOREPLIFETIME_CONDITION(URangeWeaponObject, bReloading, COND_SkipOwner);
}

int32 URangeWeaponObject::CalculateDamageWithDistance(const FVector& Start, const FVector& End, float Damage)
{
    int32 const Distance = UKismetMathLibrary::Vector_Distance(Start, End) - RangeWeaponData.MaxDamageDistance;
    int32 const DamageReduction = Distance > 0 ? Distance / (RangeWeaponData.DamageDrop * 100 ) : 0;
    return UKismetMathLibrary::Max(Damage - DamageReduction, Damage / 2);;
}

void URangeWeaponObject::PlayerWeaponEffectors()
{
    Super::PlayerWeaponEffectors();
    
    UGameplayStatics::SpawnEmitterAttached(RangeWeaponData.FireParticle, CharacterOwner->GetWeaponSkeletalMeshComponent(), "Muzzle");
}

void URangeWeaponObject::OnRep_Owner()
{
    Super::OnRep_Owner();

    CharacterOwner->GetWeaponManager()->OnSelectWeaponEvent.AddDynamic(this, &URangeWeaponObject::ClearReload);
    CharacterOwner->GetStaminaComponent()->OnStaminaUsedEvent.AddDynamic(this, &URangeWeaponObject::ClearReload);
}

bool URangeWeaponObject::UseWeapon()
{
    if(Super::UseWeapon())
    {
        if(!IsOtherPlayer())
        {
            bUseWeapon = true;
            CurrentAmmoInClip--;
        }

        FHitResult OutHit;
        DropLineTrace(OutHit);
        
        if(GetAuthority())
        {
            ApplyDamageByTrace(OutHit);
        }
    }
    return true;
}

void URangeWeaponObject::StopUseWeapon()
{
    Super::StopUseWeapon();

    CurrentSpread = 0.f;
}

bool URangeWeaponObject::IsAbleToUseWeapon()
{
    return Super::IsAbleToUseWeapon() && (IsOtherPlayer() || CurrentAmmoInClip > 0) && !bReloading;
}

void URangeWeaponObject::StopRateDelay()
{
    Super::StopRateDelay();
    
    if(RangeWeaponData.CanAutoFire && bUseWeapon)
    {
        UseWeapon();
    }
}

FVector URangeWeaponObject::GetShootDirection()
{
    FVector RotateAroundVector = CharacterOwner->GetActorForwardVector().RotateAngleAxis(CharacterOwner->GetLookUpYaw(), CharacterOwner->GetActorRightVector());
    RotateAroundVector.Z *= -1;
    
    CurrentSpread += RangeWeaponData.MaxSpread / RangeWeaponData.MaxAmmoInWeapon;

    /** Rotate trace with horizontal */
    FVector const HorizontalRotate = RotateAroundVector.RotateAngleAxis(UKismetMathLibrary::RandomFloatInRangeFromStream(CurrentSpread * -1, CurrentSpread, RangeWeaponData.FireRandomStream), FRotationMatrix(RotateAroundVector.Rotation()).GetScaledAxis(EAxis::Y));

    /** reottae trace with use up vector */
    FVector const VerticalRotate = HorizontalRotate.RotateAngleAxis(UKismetMathLibrary::RandomFloatInRangeFromStream(CurrentSpread * -1, CurrentSpread, RangeWeaponData.FireRandomStream), FRotationMatrix(RotateAroundVector.Rotation()).GetScaledAxis(EAxis::Z));

    return VerticalRotate;
}

void URangeWeaponObject::DropLineTrace(FHitResult& OutHit)
{
    //FVector const TraceStart = CharacterOwner->GetWeaponSkeletalMeshComponent()->GetSocketLocation("Muzzle");
    FVector const TraceStart = CharacterOwner->CameraComponent->GetComponentLocation();
    FVector const TraceEnd = GetShootDirection() * RangeWeaponData.RangeOfUse + TraceStart;

    /** Init Collision Quary param */
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(CharacterOwner);

    /** Init collision object param */
    FCollisionObjectQueryParams CollisionObjectQueryParams;
    CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
    CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

    /** Drop line trace end call multicast rpc function */
    GetWorld()->LineTraceSingleByObjectType(OutHit, TraceStart, TraceEnd, CollisionObjectQueryParams, Params);
   // GetTraceInfoDebugger(TraceStart, OutHit.TraceEnd, OutHit.Location);

    DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Purple, false, 1.f);
    DrawDebugSphere(GetWorld(), OutHit.Location, 3.f, 8, FColor::Purple, false, 1.f);
}

void URangeWeaponObject::ApplyDamageByTrace(const FHitResult& HitResult)
{
    /** Damage with calculate on distance */
    int32 const NewDamage = CalculateDamageWithDistance(HitResult.TraceStart, HitResult.ImpactPoint, GetBaseDamage());

    /** Find Direction Unit Vector */
    FVector const UnitVector = UKismetMathLibrary::GetDirectionUnitVector(HitResult.TraceEnd, HitResult.TraceStart);
    UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), NewDamage, UnitVector, HitResult, CharacterOwner->GetController(),CharacterOwner, UDamageType::StaticClass());
}


void URangeWeaponObject::GetTraceInfoDebugger_Implementation(FVector Start, FVector End, FVector Center)
{
    DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 1.f);
    DrawDebugSphere(GetWorld(), Center, 6.f, 8, FColor::Purple, false, 1.f);
}

void URangeWeaponObject::OwnerDead(AController* OldController)
{
    Super::OwnerDead(OldController);

    ClearReload(true);
}

bool URangeWeaponObject::IsAbleReload()
{
    /** if current ammo in clip < max ammo in clip and we have ammo in storage */
    return CurrentAmmoInClip < RangeWeaponData.MaxAmmoInWeapon && CurrentAmmoInStorage > 0 && !bReloading;
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
        
        /** Call on owning clinet for instant recharge responce */
        ReloadStart();
    }
}

void URangeWeaponObject::ClearReload(bool const Clear)
{
    if(Clear)
    {
        GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
        bReloading = false;
    }
}

void URangeWeaponObject::OnRep_Reloading()
{
    OnReloadingEvent.Broadcast(bReloading);
}

void URangeWeaponObject::ReloadStart()
{
    bReloading = true;
    
    OnRep_Reloading();
    
    GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &URangeWeaponObject::ReloadFinish,  RangeWeaponData.ReloadTime, false);
}

void URangeWeaponObject::ReloadFinish()
{
    GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
    bReloading = false;

    int32 const NeedAmmoFromMaxClip = RangeWeaponData.MaxAmmoInWeapon - CurrentAmmoInClip;
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

FString URangeWeaponObject::GetAmmoStats()
{
    return FString::FromInt(CurrentAmmoInClip) + " / " + FString::FromInt(CurrentAmmoInStorage);
}