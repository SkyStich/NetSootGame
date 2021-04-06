// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetWorkShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Objects/WeaponObject/MainWeaponObject.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "Objects/MainRangeWeapon/RangeWeaponObject.h"
#include "SingletonClass/GameSingletonClass.h"

ANetWorkShooterCharacter::ANetWorkShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
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
	
	MultiplyAngleToUseRangeWeapon = 1.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());

	/** Create and attach weapon skeletal mesh to socket */
	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));

	WeaponManagerComponent = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManager"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FistPersonMesh"));
	FirstPersonMesh->SetupAttachment(RootComponent);
}

void ANetWorkShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetLocalRole() == ROLE_Authority)
	{
		if(Controller)
			LookUpYaw = Controller->GetControlRotation().Vector().Rotation().Pitch;
	}
}

void ANetWorkShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->HealthEndedEvent.AddDynamic(this, &ANetWorkShooterCharacter::CharacterDead);
	StaminaComponent->OnStaminaEndedEvent.AddDynamic(this, &ANetWorkShooterCharacter::StopUseStamina);
	
	WeaponManagerComponent->OnSelectWeaponEvent.AddDynamic(this, &ANetWorkShooterCharacter::WeaponSelected);
		
	WeaponSkeletalMeshComponent->AttachToComponent(GetLocalMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "STK_RightArmWeaponSocket");

	if(IsLocallyControlled())
	{
		GetMesh()->DestroyComponent();
	}
	else 
	{
		FirstPersonMesh->DestroyComponent();
	}
}

void ANetWorkShooterCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	WeaponManagerComponent->OnCurrentWeaponChangedEvent.AddDynamic(this, &ANetWorkShooterCharacter::NewCurrentWeapon);
}

void ANetWorkShooterCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ANetWorkShooterCharacter, LookUpYaw);
	DOREPLIFETIME(ANetWorkShooterCharacter, MultiplyAngleToUseRangeWeapon);
	DOREPLIFETIME(ANetWorkShooterCharacter, bCrouchInCoolDawn);
}

void ANetWorkShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	/** Bind on jump event */
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	/** Begin bind crouch */
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ANetWorkShooterCharacter::CrouchPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ANetWorkShooterCharacter::CrouchReleased);
	/** Eng bind Crouch */

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

void ANetWorkShooterCharacter::CrouchPressed()
{
	if(bCrouchInCoolDawn) return;
	
	if(GetCharacterMovement()->IsCrouching())
	{
		UnCrouch(false);
	}
	else
	{
		if(!GetCharacterMovement()->IsFalling())
		{
			Crouch(true);
		}
	}
}

void ANetWorkShooterCharacter::CrouchReleased()
{
	if(bCrouchInCoolDawn) return;
	
	if(GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
}

void ANetWorkShooterCharacter::NewCurrentWeapon(UMainWeaponObject* NewWeapon, UMainWeaponObject* OldWeapon)
{
	auto TempMesh = NewWeapon->GetWeaponMesh();
	if(TempMesh.IsPending())
	{
		auto const AssetSoftObject = TempMesh.ToSoftObjectPath();

		/** Sync loand mesh in memory */
		TempMesh = Cast<USkeletalMesh>(UGameSingletonClass::Get().AssetLoader.LoadSynchronous(AssetSoftObject));
	}
	WeaponSkeletalMeshComponent->SetSkeletalMesh(TempMesh.Get());
}

void ANetWorkShooterCharacter::WeaponSelected(bool NewState)
{
	if(GetLocalRole() != ROLE_Authority)
	{
		if(NewState)
		{
			StopUseStamina();
		}
	}
}

USkeletalMeshComponent* ANetWorkShooterCharacter::GetLocalMesh()
{
	return IsLocallyControlled() ? FirstPersonMesh : GetMesh();
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

void ANetWorkShooterCharacter::ReloadPressed()
{
	auto const TempCurrentWeapon = Cast<URangeWeaponObject>(WeaponManagerComponent->GetCurrentWeapon());
	if(TempCurrentWeapon)
	{
		TempCurrentWeapon->ReloadWeapon();
	}
}

void ANetWorkShooterCharacter::CharacterDead(AController* OldController)
{
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetLocalMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if(IsLocallyControlled())
	{
		FirstPersonMesh->SetSkeletalMesh(ThirdPersonMesh);
	}
	
	if(GetLocalRole() == ROLE_Authority)
	{
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
		GetLocalMesh()->SetSimulatePhysics(true);
	}
}

void ANetWorkShooterCharacter::StartUseStamina()
{
	if(!WeaponManagerComponent->GetCurrentWeapon()->GetUseWeapon())
	{
		StaminaComponent->ClientStartUseStamina();
		Server_ChangeMovementSpeed(680.f);
	}
}

void ANetWorkShooterCharacter::StopUseStamina()
{
	StaminaComponent->ClientStopUseStamina();
	Server_ChangeMovementSpeed(440.f);
}

void ANetWorkShooterCharacter::Server_ChangeMovementSpeed_Implementation(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void ANetWorkShooterCharacter::ChangeAngleWithMovementState(float const ProcentToBaseAngle)
{
	MultiplyAngleToUseRangeWeapon += ProcentToBaseAngle;
}

void ANetWorkShooterCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	//if(!bCrouchInCoolDawn)
	//{
		if(!GetCharacterMovement()->IsFalling())
		{
			Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

			if(GetLocalRole() == ROLE_Authority)
			{
				ChangeAngleWithMovementState(-0.25f);
				bCrouchInCoolDawn = true;
				GetWorld()->GetTimerManager().SetTimer(CrouchCoolDawnHandle, this, &ANetWorkShooterCharacter::CrouchCoolDawnRefresh, 1.f, false);
			}
			OnStartCrouchEvent.Broadcast();
		}
	//}
}

void ANetWorkShooterCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	//if(!bCrouchInCoolDawn)
	//{
		Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
        
		if(GetLocalRole() == ROLE_Authority)
		{
			ChangeAngleWithMovementState(0.25f);
			bCrouchInCoolDawn = true;
			GetWorld()->GetTimerManager().SetTimer(CrouchCoolDawnHandle, this, &ANetWorkShooterCharacter::CrouchCoolDawnRefresh, 1.f, false);
		}
		OnEndCrouchEvent.Broadcast();
	//}
}

void ANetWorkShooterCharacter::CrouchCoolDawnRefresh()
{
	bCrouchInCoolDawn = false;
	GetWorld()->GetTimerManager().ClearTimer(CrouchCoolDawnHandle);
}





