// Neon Ascendant - Weapon Data Structure
// Place in: Source/NeonAscendantCore/Public/Data/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

/**
 * Weapon category enum
 */
UENUM(BlueprintType)
enum class EWeaponCategory : uint8
{
    Pistol,
    SMG,
    AssaultRifle,
    Railgun,
    PlasmaRifle,
    Experimental
};

/**
 * Weapon rarity tier
 */
UENUM(BlueprintType)
enum class EWeaponRarity : uint8
{
    Common,
    Uncommon,
    Rare,
    Epic,
    Ascendant
};

/**
 * Weapon affix modifier
 */
USTRUCT(BlueprintType)
struct NEONASCENDANTCORE_API FWeaponAffix
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString AffixName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ModifierValue = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString AffectedStat;
};

/**
 * Data table row for weapon data (for CSV import)
 */
USTRUCT(BlueprintType)
struct NEONASCENDANTCORE_API FWeaponTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EWeaponCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DamageProfile;
};

/**
 * Primary data asset for weapon definitions
 */
UCLASS(BlueprintType)
class NEONASCENDANTCORE_API UWeaponDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UWeaponDataAsset()
    {
        PrimaryAssetType = FPrimaryAssetType("Weapon");
    }

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(PrimaryAssetType, GetFName());
    }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    FString WeaponName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    EWeaponCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (MultiLine = true))
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    FString DamageProfile;

    // Runtime generated properties
    UPROPERTY(BlueprintReadOnly, Category = "Loot")
    EWeaponRarity Rarity = EWeaponRarity::Common;

    UPROPERTY(BlueprintReadOnly, Category = "Loot")
    TArray<FWeaponAffix> Affixes;

    FString GetName() const { return WeaponName; }
};

