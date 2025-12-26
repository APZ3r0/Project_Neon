#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NeonGameMode.generated.h"

class UMissionGenerator;
class ANeonEnemy;

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

	UFUNCTION(BlueprintPure, Category = "Mission")
	UMissionGenerator* GetMissionGenerator() const { return MissionGenerator; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Mission")
	TObjectPtr<UMissionGenerator> MissionGenerator;

	// Blueprint-assignable enemy class for spawning
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mission")
	TSubclassOf<class ANeonEnemy> EnemyClass;

	// Spawn points for enemies (placed in level)
	UPROPERTY(BlueprintReadOnly, Category = "Mission")
	TArray<FVector> CachedSpawnPoints;
};
