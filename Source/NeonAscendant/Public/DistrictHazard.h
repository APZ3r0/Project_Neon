#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DistrictHazard.generated.h"

class ANeonCharacter;

// Hazard type based on district data
UENUM(BlueprintType)
enum class EHazardType : uint8
{
	Thermal = 0 UMETA(DisplayName = "Thermal"),
	Electrical = 1 UMETA(DisplayName = "Electrical"),
	Toxic = 2 UMETA(DisplayName = "Toxic"),
	Radiation = 3 UMETA(DisplayName = "Radiation"),
	Cryogenic = 4 UMETA(DisplayName = "Cryogenic")
};

UCLASS()
class NEONASCENDANT_API ADistrictHazard : public AActor
{
	GENERATED_BODY()

public:
	ADistrictHazard();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Hazard configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	EHazardType HazardType = EHazardType::Thermal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	float DamagePerSecond = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	float EffectRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	bool bIsActive = true;

	// Visual indicator
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	FColor HazardColor = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	float VisualIntensity = 1.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* HazardVolume = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UParticleSystemComponent* HazardEffect = nullptr;

	// Damage tracking to prevent spam
	UPROPERTY(BlueprintReadOnly, Category = "Hazard")
	TMap<AActor*, double> LastDamageTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	float DamageTickRate = 1.0f; // Damage every X seconds

	virtual void OnHazardBeginOverlap(
		class UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	virtual void OnHazardEndOverlap(
		class UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void ApplyHazardDamage(AActor* HitActor);
	void CreateHazardEffects();
	FString GetHazardTypeName() const;
};
