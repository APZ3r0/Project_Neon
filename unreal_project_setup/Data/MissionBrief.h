// Neon Ascendant - Mission Brief Structure
// Place in: Source/NeonAscendantCore/Public/Data/

#pragma once

#include "CoreMinimal.h"
#include "MissionBrief.generated.h"

class UDistrictDataAsset;
class UFactionDataAsset;
class UArchetypeDataAsset;
class UWeaponDataAsset;
class UImplantDataAsset;

/**
 * Mission brief structure - generated procedurally
 * Ported from Python MissionBrief dataclass
 */
USTRUCT(BlueprintType)
struct NEONASCENDANTCORE_API FMissionBrief
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TSoftObjectPtr<UDistrictDataAsset> District;

    UPROPERTY(BlueprintReadOnly)
    TSoftObjectPtr<UFactionDataAsset> Opposition;

    UPROPERTY(BlueprintReadOnly)
    TSoftObjectPtr<UArchetypeDataAsset> Archetype;

    UPROPERTY(BlueprintReadOnly)
    TSoftObjectPtr<UWeaponDataAsset> PrimaryWeapon;

    UPROPERTY(BlueprintReadOnly)
    TSoftObjectPtr<UImplantDataAsset> BackupImplant;

    UPROPERTY(BlueprintReadOnly)
    TSoftObjectPtr<class UAbilityDataAsset> FeaturedAbility;

    UPROPERTY(BlueprintReadOnly)
    FString Complication;

    UPROPERTY(BlueprintReadOnly)
    FString ExtractionCondition;

    // Helper: Convert to markdown string (for debugging/documentation)
    UFUNCTION(BlueprintCallable)
    FString ToMarkdown() const;
};

