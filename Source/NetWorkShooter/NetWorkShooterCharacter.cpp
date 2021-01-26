// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetWorkShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Objects/WeaponObject/MainWeaponObject.h"
#include "Net/UnrealNetwork.h"
#include "Objects/MainRangeWeapon/RangeWeaponObject.h"

ANetWorkShooterCharacter::ANetWorkShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	/** Create and attach weapon skeletal mesh to socket */
	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	WeaponSkeletalMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "STK_RightArmWeaponSocket");
	WeaponSkeletalMeshComponent->SetSkeletalMesh(CurrentWeaponMesh);
	
	WeaponManagerComponent = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManager"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ANetWorkShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->HealthEndedEvent.AddDynamic(this, &ANetWorkShooterCharacter::CharacterDead);
}

void ANetWorkShooterCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetWorkShooterCharacter, CurrentWeaponMesh);
}

void ANetWorkShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	/** Bind on jump event */
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	/** bind on use weapon event */
	PlayerInputComponent->BindAction("UseWeapon", IE_Pressed , this, &ANetWorkShooterCharacter::UseWeaponPressed);
	PlayerInputComponent->BindAction("UseWeapon", IE_Released , this, &ANetWorkShooterCharacter::UseWeaponReleased);

	/** Bind on reload range weapon */
	PlayerInputComponent->BindAction("Reload", IE_Pressed , this, &ANetWorkShooterCharacter::ReloadPressed);
	
	/** Bint on move event */
	PlayerInputComponent->BindAxis("MoveForward", this, &ANetWorkShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANetWorkShooterCharacter::MoveRight);

	/** Bind on turn and move event */
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ANetWorkShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ANetWorkShooterCharacter::LookUpAtRate);
}

void ANetWorkShooterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANetWorkShooterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ANetWorkShooterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ANetWorkShooterCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ANetWorkShooterCharacter::OnRep_CurrentWeaponMesh()
{
	WeaponSkeletalMeshComponent->SetSkeletalMesh(CurrentWeaponMesh);
}

void ANetWorkShooterCharacter::UseWeaponPressed()
{
	WeaponManagerComponent->ServerStartUseWeapon();
}

void ANetWorkShooterCharacter::UseWeaponReleased()
{
	WeaponManagerComponent->ServerStopUseWeapon();
}

void ANetWorkShooterCharacter::ReloadPressed()
{
	if(Cast<URangeWeaponObject>(WeaponManagerComponent->GetCurrentWeapon()))
	{
		ServerReloading();
	}
}

void ANetWorkShooterCharacter::ServerReloading_Implementation()
{
	Cast<URangeWeaponObject>(WeaponManagerComponent->GetCurrentWeapon())->ReloadStart();
}

void ANetWorkShooterCharacter::CharacterDead()
{
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if(GetLocalRole() == ROLE_Authority)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		/** Create Timer delegate and bind on lambda. Lambda create work for destroy player */
		FTimerDelegate TimerCallBack;

		/** Lambda -> void, instigate to drop dead of player and destroy of him */
		TimerCallBack.BindLambda([&]() -> void
			{
			UMainWeaponObject* MainDeadPlayer;
			WeaponManagerComponent->GetWeaponByCategory(EEquipmentSlot::MainWeapon, MainDeadPlayer);	
			WeaponManagerComponent->DropWeaponToWorld(MainDeadPlayer);
			Destroy();
			});

		/** Start Timer */
		FTimerHandle DeadHandle;
		GetWorld()->GetTimerManager().SetTimer(DeadHandle, TimerCallBack, 5.f, false);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(true);
	}
}

