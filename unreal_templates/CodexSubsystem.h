// Neon Ascendant - Codex Subsystem
// Place in: Source/NeonAscendantCore/Public/Subsystems/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CodexSubsystem.generated.h"

class UArchetypeDataAsset;
class UWeaponDataAsset;
class UDistrictDataAsset;
class UFactionDataAsset;
class UAbilityDataAsset;
class UImplantDataAsset;

/**
 * Central subsystem for accessing all game data (codex)
 * Replaces the Python build_codex() function
 */
UCLASS()
class NEONASCENDANTCORE_API UCodexSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // USubsystem interface
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Archetype accessors
    UFUNCTION(BlueprintCallable, Category = "Codex|Archetypes")
    TArray<UArchetypeDataAsset*> GetAllArchetypes() const;

    UFUNCTION(BlueprintCallable, Category = "Codex|Archetypes")
    UArchetypeDataAsset* GetArchetypeByName(const FString& Name) const;

    UFUNCTION(BlueprintCallable, Category = "Codex|Archetypes")
    UArchetypeDataAsset* GetArchetypeByPrimaryAssetId(const FPrimaryAssetId& AssetId) const;

    // Weapon accessors
    UFUNCTION(BlueprintCallable, Category = "Codex|Weapons")
    TArray<UWeaponDataAsset*> GetAllWeapons() const;

    UFUNCTION(BlueprintCallable, Category = "Codex|Weapons")
    UWeaponDataAsset* GetWeaponByName(const FString& Name) const;

    // District accessors
    UFUNCTION(BlueprintCallable, Category = "Codex|Districts")
    TArray<UDistrictDataAsset*> GetAllDistricts() const;

    UFUNCTION(BlueprintCallable, Category = "Codex|Districts")
    UDistrictDataAsset* GetDistrictByName(const FString& Name) const;

    // Faction accessors
    UFUNCTION(BlueprintCallable, Category = "Codex|Factions")
    TArray<UFactionDataAsset*> GetAllFactions() const;

    UFUNCTION(BlueprintCallable, Category = "Codex|Factions")
    UFactionDataAsset* GetFactionByName(const FString& Name) const;

    // Ability accessors
    UFUNCTION(BlueprintCallable, Category = "Codex|Abilities")
    TArray<UAbilityDataAsset*> GetAllAbilities() const;

    UFUNCTION(BlueprintCallable, Category = "Codex|Abilities")
    UAbilityDataAsset* GetAbilityByName(const FString& Name) const;

    // Implant accessors
    UFUNCTION(BlueprintCallable, Category = "Codex|Implants")
    TArray<UImplantDataAsset*> GetAllImplants() const;

    UFUNCTION(BlueprintCallable, Category = "Codex|Implants")
    UImplantDataAsset* GetImplantByName(const FString& Name) const;

    // Utility: Get random item from collection
    template<typename T>
    T* GetRandom(const TArray<T*>& Items) const;

    // Check if codex is loaded
    UFUNCTION(BlueprintCallable, Category = "Codex")
    bool IsCodexLoaded() const { return bCodexLoaded; }

private:
    // Load all codex data
    void LoadCodexData();

    // Load specific asset type
    template<typename T>
    void LoadAssetsOfType(const FPrimaryAssetType& AssetType, TArray<T*>& OutArray);

    // Cached data
    UPROPERTY()
    TArray<UArchetypeDataAsset*> Archetypes;

    UPROPERTY()
    TArray<UWeaponDataAsset*> Weapons;

    UPROPERTY()
    TArray<UDistrictDataAsset*> Districts;

    UPROPERTY()
    TArray<UFactionDataAsset*> Factions;

    UPROPERTY()
    TArray<UAbilityDataAsset*> Abilities;

    UPROPERTY()
    TArray<UImplantDataAsset*> Implants;

    bool bCodexLoaded = false;
};

// Template implementation
template<typename T>
T* UCodexSubsystem::GetRandom(const TArray<T*>& Items) const
{
    if (Items.Num() == 0)
    {
        return nullptr;
    }

    const int32 Index = FMath::RandRange(0, Items.Num() - 1);
    return Items[Index];
}

