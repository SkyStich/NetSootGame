// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWeaponObject.h"
#include "Engine/NetDriver.h"
#include "Net/UnrealNetwork.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"

UMainWeaponObject::UMainWeaponObject()
{
    
}

bool UMainWeaponObject::GetAuthority() const
{
    return (GetWorld() == nullptr) || (GetWorld()->GetNetMode() != NM_Client) || GetWorld()->IsPlayingReplay();
}

int32 UMainWeaponObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
    return CharacterOwner ? CharacterOwner->GetFunctionCallspace(Function, Stack) : FunctionCallspace::Local;
}

bool UMainWeaponObject::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
    if(!CharacterOwner) return false;
    
    UNetDriver* NetDriver = CharacterOwner->GetNetDriver();
    if(!NetDriver) return false;

    NetDriver->ProcessRemoteFunction(CharacterOwner, Function, Parms, OutParms, Stack, this);
    return true;
}

void UMainWeaponObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UMainWeaponObject, CharacterOwner);
    DOREPLIFETIME_CONDITION(UMainWeaponObject, WeaponName, COND_OwnerOnly);
    DOREPLIFETIME(UMainWeaponObject, SlotCategory);
}

void UMainWeaponObject::PostInitProperties()
{
    Super::PostInitProperties();

    /** Called begin play function if world be created */
    if(GetOuter() && GetOuter()->GetWorld()) BeginPlay();
}

void UMainWeaponObject::BeginPlay()
{

}

bool UMainWeaponObject::UseWeapon()
{
    if(this->IsAbleToUseWeapon() && GetAuthority())
    {
        bUseWeapon = true;
        GetWorld()->GetTimerManager().SetTimer(UseWeaponHandle, this, &UMainWeaponObject::StopRateDelay, GetDelayBeforeUse());
        return true;
    }
   return false;
}

bool UMainWeaponObject::IsAbleToUseWeapon()
{
    return !GetWorld()->GetTimerManager().IsTimerActive(UseWeaponHandle);
}

void UMainWeaponObject::StopRateDelay()
{
    GetWorld()->GetTimerManager().ClearTimer(UseWeaponHandle);
}

void UMainWeaponObject::StopUseWeapon()
{
    bUseWeapon = false;
}

