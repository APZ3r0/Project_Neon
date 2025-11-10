// Neon Ascendant - Ability Data Structure
// Place in: Source/NeonAscendantCore/Public/Data/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "AbilityData.generated.h"

/**
 * Ability damage type
 */
UENUM(BlueprintType)
enum class EAbilityDamageType : uint8
{
    None,
    Electric,
    Cyber,
    Nanotech,
    Kinetic
};

/**
 * Data table row for ability data (for CSV import)
 */
USTRUCT(BlueprintType)
struct NEONASCENDANTCORE_API FAbilityTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CooldownSeconds = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EAbilityDamageType DamageType = EAbilityDamageType::None;
};

/**
 * Primary data asset for ability definitions
 */
UCLASS(BlueprintType)
class NEONASCENDANTCORE_API UAbilityDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UAbilityDataAsset()
    {
        PrimaryAssetType = FPrimaryAssetType("Ability");
    }

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(PrimaryAssetType, GetFName());
    }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FString AbilityName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (MultiLine = true))
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    float CooldownSeconds = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    EAbilityDamageType DamageType = EAbilityDamageType::None;

    // GAS Ability class reference
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TSubclassOf<class UGameplayAbility> AbilityClass;

    // Synergy stat (for simulation system)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Synergy")
    FString SynergyStat;

    FString GetName() const { return AbilityName; }
};

