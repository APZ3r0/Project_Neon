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
	virtual void Tick(float DeltaTime) override;
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

	// Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ANeonWeapon> StartingWeaponClass;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<ANeonWeapon> CurrentWeapon;

private:
	float DefaultWalkSpeed = 600.0f;

	void UpdateCameraMode();
};
