#include "NeonEnemyController.h"
#include "NeonEnemy.h"
#include "NeonCharacter.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ANeonEnemyController::ANeonEnemyController()
{
	PrimaryActorTick.TickInterval = 0.2f;
}

void ANeonEnemyController::BeginPlay()
{
	Super::BeginPlay();

	EnemyCharacter = Cast<ANeonEnemy>(GetPawn());
	PlayerCharacter = Cast<ANeonCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!EnemyCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANeonEnemyController::BeginPlay - Pawn is not ANeonEnemy!"));
	}

	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANeonEnemyController::BeginPlay - Could not find player character!"));
	}

	// Start in patrol state
	ChangeAIState(EEnemyAIState::Patrol);
	CurrentPatrolTarget = GetRandomPatrolPoint();
}

void ANeonEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!EnemyCharacter || !PlayerCharacter || EnemyCharacter->bIsDead)
	{
		ChangeAIState(EEnemyAIState::Dead);
		return;
	}

	UpdateAIBehavior();
}

void ANeonEnemyController::UpdateAIBehavior()
{
	// Check if we can see the target
	float DistanceToPlayer = FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
	bool bCanSeePlayer = DistanceToPlayer < DetectionRange && CanSeeTarget();
	bool bPlayerInAttackRange = IsTargetInRange();

	// State transitions
	switch (CurrentAIState)
	{
		case EEnemyAIState::Patrol:
		{
			if (bCanSeePlayer)
			{
				LastKnownPlayerLocation = PlayerCharacter->GetActorLocation();
				ChangeAIState(EEnemyAIState::Engaged);
			}
			UpdatePatrolBehavior();
			break;
		}

		case EEnemyAIState::Investigate:
		{
			if (bCanSeePlayer)
			{
				LastKnownPlayerLocation = PlayerCharacter->GetActorLocation();
				ChangeAIState(EEnemyAIState::Engaged);
			}
			else if (GetWorld()->GetTimeSeconds() - StateChangeTime > InvestigationDuration)
			{
				// Investigation timeout - return to patrol
				ChangeAIState(EEnemyAIState::Patrol);
			}
			UpdateInvestigateBehavior();
			break;
		}

		case EEnemyAIState::Engaged:
		{
			if (bCanSeePlayer)
			{
				LastKnownPlayerLocation = PlayerCharacter->GetActorLocation();

				// Check if should retreat
				float HealthPercent = EnemyCharacter->CurrentHealth / EnemyCharacter->MaxHealth;
				if (HealthPercent < RetreatHealthThreshold)
				{
					ChangeAIState(EEnemyAIState::Retreat);
				}
			}
			else if (DistanceToPlayer > LostTargetDistance)
			{
				// Lost target - investigate last known location
				ChangeAIState(EEnemyAIState::Investigate);
			}
			UpdateEngagedBehavior();
			break;
		}

		case EEnemyAIState::Retreat:
		{
			// Check if health recovered enough or player lost
			float HealthPercent = EnemyCharacter->CurrentHealth / EnemyCharacter->MaxHealth;
			if (bCanSeePlayer && HealthPercent > RetreatHealthThreshold * 1.5f)
			{
				// Recovered - re-engage
				ChangeAIState(EEnemyAIState::Engaged);
			}
			else if (!bCanSeePlayer && DistanceToPlayer > LostTargetDistance)
			{
				// Lost player while retreating - go back to patrol
				ChangeAIState(EEnemyAIState::Patrol);
			}
			UpdateRetreatBehavior();
			break;
		}

		case EEnemyAIState::Dead:
		{
			StopMovement();
			break;
		}
	}
}

void ANeonEnemyController::UpdatePatrolBehavior()
{
	if (!EnemyCharacter)
	{
		return;
	}

	// Move to patrol point
	float DistanceToPatrol = FVector::Dist(EnemyCharacter->GetActorLocation(), CurrentPatrolTarget);

	if (DistanceToPatrol < 200.0f)
	{
		// Reached patrol point, pick new one
		CurrentPatrolTarget = GetRandomPatrolPoint();
	}

	EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	MoveToLocation(CurrentPatrolTarget, 100.0f);
}

void ANeonEnemyController::UpdateInvestigateBehavior()
{
	if (!EnemyCharacter)
	{
		return;
	}

	// Move toward last known location
	EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed * 1.5f;
	MoveToLocation(LastKnownPlayerLocation, 100.0f);
}

void ANeonEnemyController::UpdateEngagedBehavior()
{
	if (!EnemyCharacter || !PlayerCharacter)
	{
		return;
	}

	// Move at combat speed toward player
	EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	MoveToActor(PlayerCharacter, 100.0f);

	// TODO: Implement shooting behavior when weapon system is integrated
	// EnemyCharacter->FireWeapon();
}

void ANeonEnemyController::UpdateRetreatBehavior()
{
	if (!EnemyCharacter)
	{
		return;
	}

	// Run away from player's last known location
	FVector RetreatDirection = (EnemyCharacter->GetActorLocation() - LastKnownPlayerLocation).GetSafeNormal();
	FVector RetreatTarget = EnemyCharacter->GetActorLocation() + RetreatDirection * 1000.0f;

	EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	MoveToLocation(RetreatTarget, 200.0f);
}

void ANeonEnemyController::ChangeAIState(EEnemyAIState NewState)
{
	if (NewState == CurrentAIState)
	{
		return;
	}

	PreviousAIState = CurrentAIState;
	CurrentAIState = NewState;
	StateChangeTime = GetWorld()->GetTimeSeconds();

	UE_LOG(LogTemp, Log, TEXT("ANeonEnemyController state changed: %s -> %s"),
		*UEnum::GetValueAsString(PreviousAIState),
		*UEnum::GetValueAsString(CurrentAIState));
}

bool ANeonEnemyController::CanSeeTarget() const
{
	if (!EnemyCharacter || !PlayerCharacter)
	{
		return false;
	}

	FVector TraceStart = GetLineTraceStart();
	FVector TraceEnd = GetLineTraceEnd();

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(EnemyCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	if (bHit && HitResult.GetActor() == PlayerCharacter)
	{
		return true;
	}

	return false;
}

bool ANeonEnemyController::IsTargetInRange() const
{
	if (!EnemyCharacter || !PlayerCharacter)
	{
		return false;
	}

	float DistanceToPlayer = FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
	return DistanceToPlayer < AttackRange;
}

FVector ANeonEnemyController::GetLineTraceStart() const
{
	if (!EnemyCharacter)
	{
		return FVector::ZeroVector;
	}

	const double HeightOffset = EnemyCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.8;
	return EnemyCharacter->GetActorLocation() + FVector(0.0, 0.0, HeightOffset);
}

FVector ANeonEnemyController::GetLineTraceEnd() const
{
	if (!PlayerCharacter)
	{
		return FVector::ZeroVector;
	}

	const double HeightOffset = PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.8;
	return PlayerCharacter->GetActorLocation() + FVector(0.0, 0.0, HeightOffset);
}

FVector ANeonEnemyController::GetRandomPatrolPoint()
{
	if (!EnemyCharacter)
	{
		return FVector::ZeroVector;
	}

	// Generate random patrol point within a radius
	FVector BaseLocation = EnemyCharacter->GetActorLocation();
	float PatrolRadius = 1500.0f;

	FVector RandomDirection = FVector(
		FMath::RandRange(-1.0f, 1.0f),
		FMath::RandRange(-1.0f, 1.0f),
		0.0f
	).GetSafeNormal();

	FVector PatrolPoint = BaseLocation + RandomDirection * FMath::RandRange(500.0f, PatrolRadius);

	// Clamp to level bounds (simple height check)
	PatrolPoint.Z = FMath::Max(PatrolPoint.Z, 100.0f);

	return PatrolPoint;
}

void ANeonEnemyController::OnEnemyDamaged(FVector DamageLocation)
{
	if (!EnemyCharacter)
	{
		return;
	}

	LastKnownPlayerLocation = DamageLocation;

	// If in patrol, switch to investigate
	if (CurrentAIState == EEnemyAIState::Patrol)
	{
		ChangeAIState(EEnemyAIState::Investigate);
	}
}
