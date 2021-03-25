// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWeaponObject.h"
#include "Engine/NetDriver.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "NetWorkShooter/NetWorkShooterCharacter.h"

UMainWeaponObject::UMainWeaponObject()
{
    
}

bool UMainWeaponObject::IsOtherPlayer() const
{
    return !CharacterOwner->Controller;
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
    DOREPLIFETIME_CONDITION(UMainWeaponObject, bUseWeapon, COND_SkipOwner);
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

void UMainWeaponObject::SetCharacterOwner(ANetWorkShooterCharacter* NewOwner)
{
    CharacterOwner = NewOwner;
    OnRep_Owner();
}

void UMainWeaponObject::OnRep_Owner()
{
    CharacterOwner->GetHealthComponent()->HealthEndedEvent.AddDynamic(this, &UMainWeaponObject::OwnerDead);  
}

void UMainWeaponObject::OwnerDead(AController* OldController)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("OwnerDead"));
    StopUseWeapon();
}

void UMainWeaponObject::PlayerWeaponEffectors()
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetUseWeaponSound(), CharacterOwner->GetActorLocation());
}

bool UMainWeaponObject::UseWeapon()
{
    if(this->IsAbleToUseWeapon())
    {
        GetWorld()->GetTimerManager().SetTimer(UseWeaponHandle, this, &UMainWeaponObject::StopRateDelay, GetDelayBeforeUse());
        OnWeaponUsedEvent.Broadcast(this);
        return true;
    }
   StopUseWeapon();
   return false;
}

bool UMainWeaponObject::IsAbleToUseWeapon()
{
    return !GetWorld()->GetTimerManager().IsTimerActive(UseWeaponHandle) && !CharacterOwner->GetWeaponManager()->GetWeaponSelecting() && !CharacterOwner->GetStaminaComponent()->GetIsStaminaUsed();
}

void UMainWeaponObject::StopRateDelay()
{
    GetWorld()->GetTimerManager().ClearTimer(UseWeaponHandle);
}

void UMainWeaponObject::StopUseWeapon()
{
    bUseWeapon = false;
}

void UMainWeaponObject::Server_UseWeapon_Implementation()
{
    UseWeapon();
}

void UMainWeaponObject::Client_UseWeapon()
{
    if(IsAbleToUseWeapon())
    {
        Server_UseWeapon();

        /** and call in client for create effects. */
        UseWeapon();
    }
}

void UMainWeaponObject::Server_StopUseWeapon_Implementation()
{
    StopUseWeapon();
}

void UMainWeaponObject::Client_StopUseWeapon()
{
    Server_StopUseWeapon();

    /** and call on client for stop play effects */
    StopUseWeapon();
}

void UMainWeaponObject::OnRep_UseWeapon()
{
    if(bUseWeapon)
    {
        UseWeapon();
    }
    else
    {
        StopUseWeapon();
    }
}

