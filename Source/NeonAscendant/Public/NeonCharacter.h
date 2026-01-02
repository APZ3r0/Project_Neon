#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NeonCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ANeonWeapon;

UCLASS()
class NEONASCENDANT_API ANeonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANeonCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Movement
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void StartCrouch();
	void StopCrouch();
	void StartSprint();
	void StopSprint();

	// Weapon
	void Fire();
	void StopFire();
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(TSubclassOf<ANeonWeapon> WeaponClass);

	// Health system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

public:
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const { return MaxHealth > 0.0f ? CurrentHealth / MaxHealth : 0.0f; }

protected:
	void Die();

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> ThirdPersonArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> ThirdPersonCamera;

	// Movement parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeedMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bUseFirstPerson = true;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting = false;

public:
	// Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ANeonWeapon> StartingWeaponClass;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<ANeonWeapon> CurrentWeapon;

private:
	// Movement configuration
	float DefaultWalkSpeed = 600.0f;

	// Camera configuration constants
	static constexpr float FirstPersonCameraHeight = 64.0f;
	static constexpr float ThirdPersonArmLength = 300.0f;
	static constexpr float ThirdPersonArmSocketOffsetY = 50.0f;
	static constexpr float ThirdPersonArmSocketOffsetZ = 75.0f;

	void UpdateCameraMode();
	FVector GetMovementDirection(EAxis::Type Axis) const;
};
