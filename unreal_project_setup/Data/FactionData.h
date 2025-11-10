// Neon Ascendant - Faction Data Structure
// Place in: Source/NeonAscendantCore/Public/Data/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "FactionData.generated.h"

/**
 * Data table row for faction data (for CSV import)
 */
USTRUCT(BlueprintType)
struct NEONASCENDANTCORE_API FFactionTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Philosophy;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SignatureTactics;  // Pipe-separated: "Tactic1|Tactic2"
};

/**
 * Primary data asset for faction definitions
 */
UCLASS(BlueprintType)
class NEONASCENDANTCORE_API UFactionDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UFactionDataAsset()
    {
        PrimaryAssetType = FPrimaryAssetType("Faction");
    }

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(PrimaryAssetType, GetFName());
    }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction")
    FString FactionName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction", meta = (MultiLine = true))
    FString Philosophy;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Faction")
    TArray<FString> SignatureTactics;

    FString GetName() const { return FactionName; }
};

