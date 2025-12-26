#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NeonEnemyController.generated.h"

class ANeonEnemy;
class ANeonCharacter;

UCLASS()
class NEONASCENDANT_API ANeonEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	ANeonEnemyController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	ANeonEnemy* EnemyCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	ANeonCharacter* PlayerCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DetectionRange = 2000.0f;

	void UpdateTargetingAndMovement();
	bool CanSeeTarget() const;

	// Perception - line trace to check if we can see the player
	FVector GetLineTraceStart() const;
	FVector GetLineTraceEnd() const;
};
