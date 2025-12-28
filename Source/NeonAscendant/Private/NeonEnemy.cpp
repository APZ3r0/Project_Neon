#include "NeonEnemy.h"
#include "NeonCharacter.h"
#include "NeonWeapon.h"
#include "NeonEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ANeonEnemy::ANeonEnemy()
{
	PrimaryActorTick.TickInterval = 0.1f;

	// Don't replicate for now
	bReplicates = false;

	// Setup character movement
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Initialize health
	CurrentHealth = MaxHealth;

	// Use AI controller
	AIControllerClass = ANeonEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ANeonEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Find the player character
	TargetPlayer = Cast<ANeonCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!TargetPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANeonEnemy::BeginPlay - Could not find player character!"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ANeonEnemy spawned and detected player at %.0f units"), 
			FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation()));
	}

	// Equip weapon
	EquipWeapon();
}

void ANeonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead || !TargetPlayer)
	{
		return;
	}

	// Simple proximity-based firing for now
	// The AI controller will handle movement
	float DistanceToPlayer = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());
	if (DistanceToPlayer < AttackRange && GetWorld()->GetTimeSeconds() - LastFireTime > FireInterval)
	{
		// Face the player
		FVector DirectionToPlayer = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator LookRotation = DirectionToPlayer.Rotation();
		SetActorRotation(FRotator(0.0f, LookRotation.Yaw, 0.0f));

		FireWeapon();
	}
}

float ANeonEnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage <= 0.0f)
	{
		return ActualDamage;
	}

	CurrentHealth -= ActualDamage;
	UE_LOG(LogTemp, Log, TEXT("ANeonEnemy took %.0f damage. Health: %.0f/%.0f"),
		ActualDamage, CurrentHealth, MaxHealth);

	// Notify AI controller of damage
	ANeonEnemyController* EnemyController = GetEnemyController();
	if (EnemyController && TargetPlayer)
	{
		EnemyController->OnEnemyDamaged(TargetPlayer->GetActorLocation());
	}

	if (CurrentHealth <= 0.0f)
	{
		Die();
	}

	return ActualDamage;
}

void ANeonEnemy::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	CurrentHealth = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("ANeonEnemy died"));

	// Drop weapon
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
		EquippedWeapon = nullptr;
	}

	// Disable movement and collision
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Simple death: ragdoll (enable physics)
	if (GetMesh())
	{
		GetMesh()->SetSimulatePhysics(true);
	}

	// Destroy after a delay
	SetLifeSpan(10.0f);
}

void ANeonEnemy::EquipWeapon()
{
	if (!WeaponClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANeonEnemy::EquipWeapon - WeaponClass not set!"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	EquippedWeapon = GetWorld()->SpawnActor<ANeonWeapon>(WeaponClass, SpawnParams);

	if (EquippedWeapon)
	{
		// Attach to hand socket
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
		UE_LOG(LogTemp, Log, TEXT("ANeonEnemy equipped weapon: %s"), *EquippedWeapon->GetName());
	}
}

void ANeonEnemy::FireWeapon()
{
	if (!EquippedWeapon || !TargetPlayer)
	{
		return;
	}

	LastFireTime = GetWorld()->GetTimeSeconds();

	// Get aim direction toward player
	// TODO: Implement custom aim parameters for enemies
	// For now, use the weapon's default fire behavior
	EquippedWeapon->Fire();
}

ANeonEnemyController* ANeonEnemy::GetEnemyController() const
{
	return Cast<ANeonEnemyController>(Controller);
}

void ANeonEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enemies don't have player input
	// This is controlled by the AI controller
}
