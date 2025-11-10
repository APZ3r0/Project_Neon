// Neon Ascendant - Mission Generation Subsystem
// Place in: Source/NeonAscendantCore/Public/Subsystems/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/MissionBrief.h"
#include "MissionGenerationSubsystem.generated.h"

class UCodexSubsystem;

/**
 * Generates mission briefs procedurally
 * Ported from Python MissionGenerator class
 */
UCLASS()
class NEONASCENDANTCORE_API UMissionGenerationSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /**
     * Generate a single mission brief
     * @param Seed Random seed (-1 for random)
     * @return Generated mission brief
     */
    UFUNCTION(BlueprintCallable, Category = "Mission Generation")
    FMissionBrief GenerateMissionBrief(int32 Seed = -1);

    /**
     * Generate multiple mission briefs
     * @param Count Number of briefs to generate
     * @param Seed Random seed (-1 for random)
     * @return Array of generated mission briefs
     */
    UFUNCTION(BlueprintCallable, Category = "Mission Generation")
    TArray<FMissionBrief> GenerateMissionBriefs(int32 Count, int32 Seed = -1);

    /**
     * Set the random seed for deterministic generation
     */
    UFUNCTION(BlueprintCallable, Category = "Mission Generation")
    void SetSeed(int32 Seed);

private:
    // Helper: Choose random item from array
    template<typename T>
    T* ChooseRandom(const TArray<T*>& Options);

    // Get random complication
    FString GetRandomComplication();

    // Get random extraction condition
    FString GetRandomExtractionCondition();

    UPROPERTY()
    UCodexSubsystem* CodexSubsystem;

    FRandomStream RandomStream;
};

