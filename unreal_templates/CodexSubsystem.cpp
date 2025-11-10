// Neon Ascendant - Codex Subsystem Implementation
// Place in: Source/NeonAscendantCore/Private/Subsystems/

#include "Subsystems/CodexSubsystem.h"
#include "Data/ArchetypeData.h"
#include "Data/WeaponData.h"
#include "Data/DistrictData.h"
#include "Data/FactionData.h"
#include "Data/AbilityData.h"
#include "Data/ImplantData.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void UCodexSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Load codex data
    LoadCodexData();
}

void UCodexSubsystem::Deinitialize()
{
    // Clear cached data
    Archetypes.Empty();
    Weapons.Empty();
    Districts.Empty();
    Factions.Empty();
    Abilities.Empty();
    Implants.Empty();

    Super::Deinitialize();
}

void UCodexSubsystem::LoadCodexData()
{
    UAssetManager* AssetManager = UAssetManager::GetIfValid();
    if (!AssetManager)
    {
        UE_LOG(LogTemp, Error, TEXT("CodexSubsystem: AssetManager not available"));
        return;
    }

    // Load all archetypes
    LoadAssetsOfType<UArchetypeDataAsset>(FPrimaryAssetType("Archetype"), Archetypes);

    // Load all weapons
    LoadAssetsOfType<UWeaponDataAsset>(FPrimaryAssetType("Weapon"), Weapons);

    // Load all districts
    LoadAssetsOfType<UDistrictDataAsset>(FPrimaryAssetType("District"), Districts);

    // Load all factions
    LoadAssetsOfType<UFactionDataAsset>(FPrimaryAssetType("Faction"), Factions);

    // Load all abilities
    LoadAssetsOfType<UAbilityDataAsset>(FPrimaryAssetType("Ability"), Abilities);

    // Load all implants
    LoadAssetsOfType<UImplantDataAsset>(FPrimaryAssetType("Implant"), Implants);

    bCodexLoaded = true;

    UE_LOG(LogTemp, Log, TEXT("CodexSubsystem: Loaded %d archetypes, %d weapons, %d districts, %d factions"),
        Archetypes.Num(), Weapons.Num(), Districts.Num(), Factions.Num());
}

template<typename T>
void UCodexSubsystem::LoadAssetsOfType(const FPrimaryAssetType& AssetType, TArray<T*>& OutArray)
{
    UAssetManager* AssetManager = UAssetManager::GetIfValid();
    if (!AssetManager)
    {
        return;
    }

    TArray<FPrimaryAssetId> AssetIds;
    AssetManager->GetPrimaryAssetIdList(AssetType, AssetIds);

    OutArray.Empty(AssetIds.Num());

    for (const FPrimaryAssetId& AssetId : AssetIds)
    {
        T* Asset = Cast<T>(AssetManager->GetPrimaryAssetObject(AssetId));
        if (Asset)
        {
            OutArray.Add(Asset);
        }
    }
}

// Explicit template instantiations
template void UCodexSubsystem::LoadAssetsOfType<UArchetypeDataAsset>(const FPrimaryAssetType&, TArray<UArchetypeDataAsset*>&);
template void UCodexSubsystem::LoadAssetsOfType<UWeaponDataAsset>(const FPrimaryAssetType&, TArray<UWeaponDataAsset*>&);
template void UCodexSubsystem::LoadAssetsOfType<UDistrictDataAsset>(const FPrimaryAssetType&, TArray<UDistrictDataAsset*>&);
template void UCodexSubsystem::LoadAssetsOfType<UFactionDataAsset>(const FPrimaryAssetType&, TArray<UFactionDataAsset*>&);
template void UCodexSubsystem::LoadAssetsOfType<UAbilityDataAsset>(const FPrimaryAssetType&, TArray<UAbilityDataAsset*>&);
template void UCodexSubsystem::LoadAssetsOfType<UImplantDataAsset>(const FPrimaryAssetType&, TArray<UImplantDataAsset*>&);

TArray<UArchetypeDataAsset*> UCodexSubsystem::GetAllArchetypes() const
{
    return Archetypes;
}

UArchetypeDataAsset* UCodexSubsystem::GetArchetypeByName(const FString& Name) const
{
    for (UArchetypeDataAsset* Archetype : Archetypes)
    {
        if (Archetype && Archetype->ArchetypeName.Equals(Name, ESearchCase::IgnoreCase))
        {
            return Archetype;
        }
    }
    return nullptr;
}

UArchetypeDataAsset* UCodexSubsystem::GetArchetypeByPrimaryAssetId(const FPrimaryAssetId& AssetId) const
{
    UAssetManager* AssetManager = UAssetManager::GetIfValid();
    if (!AssetManager)
    {
        return nullptr;
    }

    return Cast<UArchetypeDataAsset>(AssetManager->GetPrimaryAssetObject(AssetId));
}

TArray<UWeaponDataAsset*> UCodexSubsystem::GetAllWeapons() const
{
    return Weapons;
}

UWeaponDataAsset* UCodexSubsystem::GetWeaponByName(const FString& Name) const
{
    for (UWeaponDataAsset* Weapon : Weapons)
    {
        if (Weapon && Weapon->WeaponName.Equals(Name, ESearchCase::IgnoreCase))
        {
            return Weapon;
        }
    }
    return nullptr;
}

TArray<UDistrictDataAsset*> UCodexSubsystem::GetAllDistricts() const
{
    return Districts;
}

UDistrictDataAsset* UCodexSubsystem::GetDistrictByName(const FString& Name) const
{
    for (UDistrictDataAsset* District : Districts)
    {
        if (District && District->DistrictName.Equals(Name, ESearchCase::IgnoreCase))
        {
            return District;
        }
    }
    return nullptr;
}

TArray<UFactionDataAsset*> UCodexSubsystem::GetAllFactions() const
{
    return Factions;
}

UFactionDataAsset* UCodexSubsystem::GetFactionByName(const FString& Name) const
{
    for (UFactionDataAsset* Faction : Factions)
    {
        if (Faction && Faction->FactionName.Equals(Name, ESearchCase::IgnoreCase))
        {
            return Faction;
        }
    }
    return nullptr;
}

TArray<UAbilityDataAsset*> UCodexSubsystem::GetAllAbilities() const
{
    return Abilities;
}

UAbilityDataAsset* UCodexSubsystem::GetAbilityByName(const FString& Name) const
{
    for (UAbilityDataAsset* Ability : Abilities)
    {
        if (Ability && Ability->AbilityName.Equals(Name, ESearchCase::IgnoreCase))
        {
            return Ability;
        }
    }
    return nullptr;
}

TArray<UImplantDataAsset*> UCodexSubsystem::GetAllImplants() const
{
    return Implants;
}

UImplantDataAsset* UCodexSubsystem::GetImplantByName(const FString& Name) const
{
    for (UImplantDataAsset* Implant : Implants)
    {
        if (Implant && Implant->ImplantName.Equals(Name, ESearchCase::IgnoreCase))
        {
            return Implant;
        }
    }
    return nullptr;
}

