#include "NeonEnemyController.h"
#include "NeonEnemy.h"
#include "NeonCharacter.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Kismet/GameplayStatics.h"

ANeonEnemyController::ANeonEnemyController()
{
	PrimaryActorTick.TickInterval = 0.2f;

	// Use pathfinding
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
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
}

void ANeonEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!EnemyCharacter || !PlayerCharacter || EnemyCharacter->bIsDead)
	{
		return;
	}

	UpdateTargetingAndMovement();
}

void ANeonEnemyController::UpdateTargetingAndMovement()
{
	if (!EnemyCharacter || !PlayerCharacter)
	{
		return;
	}

	float DistanceToPlayer = FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());

	if (DistanceToPlayer < DetectionRange && CanSeeTarget())
	{
		// Move toward player
		MoveToActor(PlayerCharacter, 100.0f);
	}
	else
	{
		// Stop moving
		StopMovement();
	}
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

FVector ANeonEnemyController::GetLineTraceStart() const
{
	if (!EnemyCharacter)
	{
		return FVector::ZeroVector;
	}

	return EnemyCharacter->GetActorLocation() + FVector(0.0f, 0.0f, EnemyCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.8f);
}

FVector ANeonEnemyController::GetLineTraceEnd() const
{
	if (!PlayerCharacter)
	{
		return FVector::ZeroVector;
	}

	return PlayerCharacter->GetActorLocation() + FVector(0.0f, 0.0f, PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.8f);
}
