#include "NeonEnemy.h"
#include "NeonCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
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
}

void ANeonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead || !TargetPlayer)
	{
		return;
	}

	// Simple AI: check distance to player
	float DistanceToPlayer = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());

	// If player is in detection range, move toward them
	if (DistanceToPlayer < DetectionRange)
	{
		FVector DirectionToPlayer = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		AddMovementInput(DirectionToPlayer, 1.0f);

		// Log periodically for debugging
		static double LastLogTime = 0.0;
		if (GetWorld()->GetTimeSeconds() - LastLogTime > 2.0)
		{
			UE_LOG(LogTemp, Log, TEXT("ANeonEnemy chasing player at distance %.0f"), DistanceToPlayer);
			LastLogTime = GetWorld()->GetTimeSeconds();
		}
	}
	else
	{
		// Patrol or idle
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}
}

void ANeonEnemy::TakeDamage(float Damage)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	CurrentHealth -= Damage;
	UE_LOG(LogTemp, Log, TEXT("ANeonEnemy took %.0f damage. Health: %.0f/%.0f"), 
		Damage, CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		Die();
	}
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

	// Disable movement and collision
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECC_Pawn);

	// Simple death: ragdoll (enable physics)
	GetMesh()->SetSimulatePhysics(true);

	// Destroy after a delay
	SetLifeSpan(10.0f);
}

void ANeonEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enemies don't have player input
	// This is controlled by the AI controller
}
