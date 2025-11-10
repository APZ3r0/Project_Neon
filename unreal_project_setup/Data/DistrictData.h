// Neon Ascendant - District Data Structure
// Place in: Source/NeonAscendantCore/Public/Data/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "DistrictData.generated.h"

/**
 * Data table row for district data (for CSV import)
 */
USTRUCT(BlueprintType)
struct NEONASCENDANTCORE_API FDistrictTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Hazards;  // Pipe-separated: "Hazard1|Hazard2|Hazard3"

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EnemyProfiles;  // Pipe-separated: "Enemy1|Enemy2"
};

/**
 * Primary data asset for district definitions
 */
UCLASS(BlueprintType)
class NEONASCENDANTCORE_API UDistrictDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UDistrictDataAsset()
    {
        PrimaryAssetType = FPrimaryAssetType("District");
    }

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(PrimaryAssetType, GetFName());
    }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "District")
    FString DistrictName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "District", meta = (MultiLine = true))
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "District")
    TArray<FString> Hazards;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "District")
    TArray<FString> EnemyProfiles;

    // Procedural generation parameters
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generation")
    TSoftObjectPtr<UWorld> BaseLevel;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generation")
    TArray<TSoftObjectPtr<UClass>> ModularPieces;

    FString GetName() const { return DistrictName; }
};

