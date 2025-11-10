// Auto-generated constants from Python prototype
// DO NOT EDIT MANUALLY - Regenerate using scripts/export_to_unreal.py

#pragma once

#include "CoreMinimal.h"
#include "GameConstants.generated.h"

UCLASS()
class NEONASCENDANTCORE_API UGameConstants : public UObject
{
    GENERATED_BODY()

public:
    // Mission Complications
    static const TArray<FString> GetComplications()
    {
        return TArray<FString>{
            TEXT("Ghost Grid instabilities cause random HUD distortion"),
            TEXT("Helix orbital overwatch sweeps disrupt cloak cycles"),
            TEXT("Dawnbreaker converts attempt to hack your cyberdeck mid-fight"),
            TEXT("Rival Ascendant strike team is pursuing the same data ghost"),
        };
    }
    
    // Extraction Conditions
    static const TArray<FString> GetExtractionConditions()
    {
        return TArray<FString>{
            TEXT("Secure a clean uplink and survive the counter-hack timer"),
            TEXT("Evacuate via hijacked mag-lev within 90 seconds of objective completion"),
            TEXT("Carry data-core physically to an Ascendant drop pod"),
            TEXT("Maintain over 50% armor integrity for premium rewards"),
        };
    }
    
    // Simulation Constants
    static constexpr int32 AbilitySynergyBonus = 2;
    static constexpr int32 ImplantSynergyBonus = 1;
    static constexpr int32 DiceRollMin = 1;
    static constexpr int32 DiceRollMax = 6;
    static constexpr int32 DifficultyVarianceMin = 0;
    static constexpr int32 DifficultyVarianceMax = 3;
};
