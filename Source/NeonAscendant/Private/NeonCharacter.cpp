#include "NeonCharacter.h"
#include "NeonWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ANeonCharacter::ANeonCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Configure capsule
	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, DefaultRotationRate, 0.0f);
	GetCharacterMovement()->JumpZVelocity = DefaultJumpZVelocity;
	GetCharacterMovement()->AirControl = DefaultAirControl;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = DefaultWalkSpeed * CrouchSpeedMultiplier;

	// Don't rotate character with camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Create third-person camera boom
	ThirdPersonArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonArm"));
	ThirdPersonArm->SetupAttachment(RootComponent);
	ThirdPersonArm->TargetArmLength = ThirdPersonArmLength;
	ThirdPersonArm->bUsePawnControlRotation = true;
	ThirdPersonArm->SocketOffset = FVector(0.0f, ThirdPersonArmSocketOffsetY, ThirdPersonArmSocketOffsetZ);

	// Create third-person camera
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(ThirdPersonArm, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;

	// Create first-person camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, FirstPersonCameraHeight));
	FirstPersonCamera->bUsePawnControlRotation = true;
}

void ANeonCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize health based on MaxHealth (respects Blueprint overrides)
	CurrentHealth = MaxHealth;

	UpdateCameraMode();

	// Equip starting weapon
	if (StartingWeaponClass)
	{
		EquipWeapon(StartingWeaponClass);
	}
}

void ANeonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ANeonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANeonCharacter::MoveRight);

	// Looking
	PlayerInputComponent->BindAxis("LookUp", this, &ANeonCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ANeonCharacter::Turn);

	// Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ANeonCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ANeonCharacter::StopCrouch);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ANeonCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ANeonCharacter::StopSprint);

	// Weapon
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ANeonCharacter::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ANeonCharacter::StopFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ANeonCharacter::Reload);
}

FVector ANeonCharacter::GetMovementDirection(EAxis::Type Axis) const
{
	if (!Controller)
	{
		return FVector::ZeroVector;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	return FRotationMatrix(YawRotation).GetUnitAxis(Axis);
}

void ANeonCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetMovementDirection(EAxis::X), Value);
	}
}

void ANeonCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetMovementDirection(EAxis::Y), Value);
	}
}

void ANeonCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ANeonCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ANeonCharacter::StartCrouch()
{
	Crouch();
}

void ANeonCharacter::StopCrouch()
{
	UnCrouch();
}

void ANeonCharacter::StartSprint()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * SprintSpeedMultiplier;
}

void ANeonCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void ANeonCharacter::Fire()
{
	if (!CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->StartFire();
}

void ANeonCharacter::StopFire()
{
	if (!CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->StopFire();
}

void ANeonCharacter::Reload()
{
	if (!CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->Reload();
}

void ANeonCharacter::EquipWeapon(TSubclassOf<ANeonWeapon> WeaponClass)
{
	if (!WeaponClass)
		return;

	UWorld* World = GetWorld();
	if (!World)
		return;

	// Destroy current weapon if exists
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}

	// Spawn new weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	CurrentWeapon = World->SpawnActor<ANeonWeapon>(WeaponClass, SpawnParams);

	if (CurrentWeapon)
	{
		// Attach to character mesh
		if (GetMesh()->DoesSocketExist(WeaponSocketName))
		{
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
		}
		else
		{
			// Fallback to camera if socket doesn't exist
			CurrentWeapon->AttachToComponent(FirstPersonCamera, FAttachmentTransformRules::SnapToTargetIncludingScale);
			CurrentWeapon->SetActorRelativeLocation(FVector(FallbackWeaponOffsetX, FallbackWeaponOffsetY, FallbackWeaponOffsetZ));
		}
	}
}

void ANeonCharacter::UpdateCameraMode()
{
	if (bUseFirstPerson)
	{
		FirstPersonCamera->SetActive(true);
		ThirdPersonCamera->SetActive(false);
		GetMesh()->SetOwnerNoSee(true);
	}
	else
	{
		FirstPersonCamera->SetActive(false);
		ThirdPersonCamera->SetActive(true);
		GetMesh()->SetOwnerNoSee(false);
	}
}
float ANeonCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage <= 0.0f)
	{
		return ActualDamage;
	}

	CurrentHealth -= ActualDamage;

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Log, TEXT("Player took %.0f damage. Health: %.0f/%.0f"),
		ActualDamage, CurrentHealth, MaxHealth);
#endif

	if (CurrentHealth <= 0.0f)
	{
		Die();
	}

	return ActualDamage;
}

void ANeonCharacter::Die()
{
#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Log, TEXT("Player died!"));
#endif

	// Disable input
	if (Controller)
	{
		Controller->UnPossess();
	}

	// Stop firing and movement
	StopFire();
	GetCharacterMovement()->DisableMovement();

	// Drop weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	// Ragdoll
	if (GetMesh())
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	// Destroy after delay
	SetLifeSpan(DeathLifespan);
}