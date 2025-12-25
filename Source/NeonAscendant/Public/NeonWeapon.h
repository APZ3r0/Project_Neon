#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeonWeapon.generated.h"

class USkeletalMeshComponent;
class UArrowComponent;

UCLASS()
class NEONASCENDANT_API ANeonWeapon : public AActor
{
	GENERATED_BODY()

public:
	ANeonWeapon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Weapon actions
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	// Weapon info
	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetCurrentAmmo() const { return CurrentAmmo; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetMaxAmmo() const { return MaxAmmo; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool IsReloading() const { return bIsReloading; }

protected:
	void Fire();
	void FinishReload();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UArrowComponent> MuzzleLocation;

	// Weapon stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Damage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float FireRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Range = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 MaxAmmo = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float ReloadTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	bool bIsAutomatic = true;

	// Weapon state
	UPROPERTY(BlueprintReadOnly, Category = "Weapon State")
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon State")
	bool bIsFiring = false;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon State")
	bool bIsReloading = false;

private:
	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;
};
