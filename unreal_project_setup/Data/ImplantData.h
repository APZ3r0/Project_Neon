// Neon Ascendant - Implant Data Structure
// Place in: Source/NeonAscendantCore/Public/Data/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "ImplantData.generated.h"

/**
 * Implant slot type
 */
UENUM(BlueprintType)
enum class EImplantSlot : uint8
{
    Cyberdeck,
    ReflexCore,
    OpticCortex,
    Other
};

/**
 * Data table row for implant data (for CSV import)
 */
USTRUCT(BlueprintType)
struct NEONASCENDANTCORE_API FImplantTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Slot;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Effects;  // Pipe-separated: "Effect1|Effect2"
};

/**
 * Primary data asset for implant definitions
 */
UCLASS(BlueprintType)
class NEONASCENDANTCORE_API UImplantDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UImplantDataAsset()
    {
        PrimaryAssetType = FPrimaryAssetType("Implant");
    }

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(PrimaryAssetType, GetFName());
    }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Implant")
    FString ImplantName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Implant")
    EImplantSlot Slot;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Implant")
    TArray<FString> Effects;

    // Synergy stat (for simulation system)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Synergy")
    FString SynergyStat;

    FString GetName() const { return ImplantName; }
    
    FString GetSlotName() const
    {
        switch (Slot)
        {
            case EImplantSlot::Cyberdeck: return TEXT("Cyberdeck");
            case EImplantSlot::ReflexCore: return TEXT("Reflex Core");
            case EImplantSlot::OpticCortex: return TEXT("Optic Cortex");
            default: return TEXT("Other");
        }
    }
};

