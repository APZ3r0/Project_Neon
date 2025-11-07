#pragma once

#include "CoreMinimal.h"
#include "MissionTypes.h"
#include "MissionGenerator.generated.h"

UCLASS(BlueprintType)
class NEONASCENDANT_API UMissionGenerator : public UObject
{
    GENERATED_BODY()

public:
    UMissionGenerator();

    UFUNCTION(BlueprintCallable, Category="Mission")
    void SeedGenerator(int32 Seed);

    UFUNCTION(BlueprintCallable, Category="Mission")
    FMissionBrief GenerateMissionBrief();

    UFUNCTION(BlueprintCallable, Category="Mission")
    void GenerateMissionBriefs(int32 Count, TArray<FMissionBrief>& OutBriefs);

private:
    FRandomStream RandomStream;
    bool bHasSeed;

    void EnsureRandomStream();
};

UCLASS(Config=Game)
class NEONASCENDANT_API UMissionGeneratorSingleton : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category="Mission")
    static UMissionGenerator* GetGenerator();

private:
    static TObjectPtr<UMissionGenerator> GeneratorInstance;
};
