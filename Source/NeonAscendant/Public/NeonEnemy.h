#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NeonEnemy.generated.h"

class ANeonCharacter;

UCLASS()
class NEONASCENDANT_API ANeonEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ANeonEnemy();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Health system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrentHealth = 100.0f;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float Damage);

	void Die();

	// Targeting and detection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DetectionRange = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 500.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	ANeonCharacter* TargetPlayer = nullptr;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool bIsDead = false;
};
