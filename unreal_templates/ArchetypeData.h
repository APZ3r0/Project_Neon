// Neon Ascendant - Archetype Data Structure
// Place in: Source/NeonAscendantCore/Public/Data/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "ArchetypeData.generated.h"

/**
 * Core stat profile for an archetype
 */
USTRUCT(BlueprintType)
struct NEONASCENDANTCORE_API FArchetypeProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "10"))
    int32 Stealth = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "10"))
    int32 Assault = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "10"))
    int32 Tech = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "10"))
    int32 Resilience = 0;

    // Helper function to get stat by name
    int32 GetStatByName(const FString& StatName) const
    {
        if (StatName.Equals(TEXT("stealth"), ESearchCase::IgnoreCase))
            return Stealth;
        if (StatName.Equals(TEXT("assault"), ESearchCase::IgnoreCase))
            return Assault;
        if (StatName.Equals(TEXT("tech"), ESearchCase::IgnoreCase))
            return Tech;
        if (StatName.Equals(TEXT("resilience"), ESearchCase::IgnoreCase))
            return Resilience;
        return 0;
    }
};

/**
 * Data table row for archetype data (for CSV import)
 */
USTRUCT(BlueprintType)
struct NEONASCENDANTCORE_API FArchetypeTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Role;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Signature;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FArchetypeProfile Profile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> AbilityNames;
};

/**
 * Primary data asset for archetype definitions
 */
UCLASS(BlueprintType)
class NEONASCENDANTCORE_API UArchetypeDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UArchetypeDataAsset()
    {
        PrimaryAssetType = FPrimaryAssetType("Archetype");
    }

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(PrimaryAssetType, GetFName());
    }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Archetype")
    FString ArchetypeName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Archetype")
    FString Role;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Archetype", meta = (MultiLine = true))
    FString Signature;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    FArchetypeProfile BaseStats;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
    TArray<TSoftObjectPtr<class UAbilityDataAsset>> Abilities;

    // Get archetype name (for compatibility)
    FString GetName() const { return ArchetypeName; }
};

