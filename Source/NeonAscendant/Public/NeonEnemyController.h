#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NeonEnemyController.generated.h"

class ANeonEnemy;
class ANeonCharacter;

// AI behavior states
UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
	Patrol = 0 UMETA(DisplayName = "Patrol"),
	Investigate = 1 UMETA(DisplayName = "Investigate"),
	Engaged = 2 UMETA(DisplayName = "Engaged"),
	Retreat = 3 UMETA(DisplayName = "Retreat"),
	Dead = 4 UMETA(DisplayName = "Dead")
};

UCLASS()
class NEONASCENDANT_API ANeonEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	ANeonEnemyController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Get current AI state
	UFUNCTION(BlueprintPure, Category = "AI")
	EEnemyAIState GetAIState() const { return CurrentAIState; }

	// Perception - receive information about damage location
	void OnEnemyDamaged(FVector DamageLocation);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	ANeonEnemy* EnemyCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	ANeonCharacter* PlayerCharacter = nullptr;

	// AI State Machine
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	EEnemyAIState CurrentAIState = EEnemyAIState::Patrol;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	EEnemyAIState PreviousAIState = EEnemyAIState::Patrol;

	// Detection parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Detection")
	float DetectionRange = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Detection")
	float LostTargetDistance = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Detection")
	float InvestigationDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Combat")
	float AttackRange = 500.0f;

	// Patrol behavior
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	float PatrolSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	float PatrolWaitTime = 2.0f;

	// Retreat behavior
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Retreat")
	float RetreatHealthThreshold = 0.25f; // Retreat when below 25% health

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	FVector LastKnownPlayerLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	double StateChangeTime = 0.0;

	void UpdateAIBehavior();
	void UpdatePatrolBehavior();
	void UpdateInvestigateBehavior();
	void UpdateEngagedBehavior();
	void UpdateRetreatBehavior();

	void ChangeAIState(EEnemyAIState NewState);
	bool CanSeeTarget() const;
	bool IsTargetInRange() const;

	// Perception - line trace to check if we can see the player
	FVector GetLineTraceStart() const;
	FVector GetLineTraceEnd() const;

	// Patrol point generation
	FVector GetRandomPatrolPoint();
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	FVector CurrentPatrolTarget = FVector::ZeroVector;
};
