#include "NeonCharacter.h"
#include "NeonWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ANeonCharacter::ANeonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Configure capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = DefaultWalkSpeed * CrouchSpeedMultiplier;

	// Don't rotate character with camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Create third-person camera boom
	ThirdPersonArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonArm"));
	ThirdPersonArm->SetupAttachment(RootComponent);
	ThirdPersonArm->TargetArmLength = 300.0f;
	ThirdPersonArm->bUsePawnControlRotation = true;
	ThirdPersonArm->SocketOffset = FVector(0.0f, 50.0f, 75.0f);

	// Create third-person camera
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(ThirdPersonArm, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;

	// Create first-person camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;
}

void ANeonCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateCameraMode();

	// Equip starting weapon
	if (StartingWeaponClass)
	{
		EquipWeapon(StartingWeaponClass);
	}
}

void ANeonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void ANeonCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ANeonCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
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
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void ANeonCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void ANeonCharacter::Reload()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

void ANeonCharacter::EquipWeapon(TSubclassOf<ANeonWeapon> WeaponClass)
{
	if (!WeaponClass)
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

	CurrentWeapon = GetWorld()->SpawnActor<ANeonWeapon>(WeaponClass, SpawnParams);

	if (CurrentWeapon)
	{
		// Attach to character mesh
		FName WeaponSocket = TEXT("hand_rSocket");
		if (GetMesh()->DoesSocketExist(WeaponSocket))
		{
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		}
		else
		{
			// Fallback to camera if socket doesn't exist
			CurrentWeapon->AttachToComponent(FirstPersonCamera, FAttachmentTransformRules::SnapToTargetIncludingScale);
			CurrentWeapon->SetActorRelativeLocation(FVector(30.0f, 10.0f, -10.0f));
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
	UE_LOG(LogTemp, Log, TEXT("Player took %.0f damage. Health: %.0f/%.0f"),
		ActualDamage, CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		Die();
	}

	return ActualDamage;
}

void ANeonCharacter::Die()
{
	UE_LOG(LogTemp, Log, TEXT("Player died!"));

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
	SetLifeSpan(10.0f);
}