#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NeonEnemy.generated.h"

class ANeonCharacter;
class ANeonWeapon;
class ANeonEnemyController;

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

	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

	// Weapon system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<ANeonWeapon> WeaponClass;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<ANeonWeapon> EquippedWeapon = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FireWeapon();

	// Targeting and detection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DetectionRange = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 500.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	ANeonCharacter* TargetPlayer = nullptr;

	// Get AI controller
	UFUNCTION(BlueprintPure, Category = "AI")
	ANeonEnemyController* GetEnemyController() const;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool bIsDead = false;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void EquipWeapon();

	// Combat state
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	double LastFireTime = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireInterval = 0.15f; // Time between shots
};
