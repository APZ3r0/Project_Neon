#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NeonGameMode.generated.h"

class UMissionGenerator;
class ANeonEnemy;
class ADistrictHazard;
struct FMissionBrief;

UCLASS()
class NEONASCENDANT_API ANeonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANeonGameMode();

protected:
	virtual void BeginPlay() override;

public:
	// Generate a new mission
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void StartNewMission();

	// Spawn enemies based on mission data
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void SpawnEnemiesForMission(const FMissionBrief& Mission, int32 EnemyCount = 3);

	// Spawn district hazards based on mission data
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void SpawnHazardsForMission(const FMissionBrief& Mission);

	UFUNCTION(BlueprintPure, Category = "Mission")
	UMissionGenerator* GetMissionGenerator() const { return MissionGenerator; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Mission")
	TObjectPtr<UMissionGenerator> MissionGenerator;

	// Blueprint-assignable enemy class for spawning
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mission")
	TSubclassOf<class ANeonEnemy> EnemyClass;

	// Blueprint-assignable hazard class for spawning
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mission")
	TSubclassOf<class ADistrictHazard> HazardClass;

	// Spawn points for enemies (placed in level)
	UPROPERTY(BlueprintReadOnly, Category = "Mission")
	TArray<FVector> CachedSpawnPoints;

	// Spawned hazards tracking
	UPROPERTY(BlueprintReadOnly, Category = "Mission")
	TArray<TObjectPtr<ADistrictHazard>> ActiveHazards;

	// Random stream for deterministic spawning
	FRandomStream SpawnRandomStream;

private:
	// Enemy spawn configuration
	static constexpr float EnemySpawnMinDistance = -2000.0f;
	static constexpr float EnemySpawnMaxDistance = 2000.0f;
	static constexpr float EnemySpawnHeightOffset = 100.0f;
	static constexpr int32 DefaultEnemyCount = 3;

	// Hazard spawn configuration
	static constexpr float HazardSpawnMinDistance = -3000.0f;
	static constexpr float HazardSpawnMaxDistance = 3000.0f;
	static constexpr float HazardSpawnHeight = 100.0f;
	static constexpr int32 MinHazardCount = 2;
	static constexpr int32 MaxHazardCount = 3;
};
