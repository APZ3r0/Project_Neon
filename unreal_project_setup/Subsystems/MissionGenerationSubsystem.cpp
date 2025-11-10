// Neon Ascendant - Mission Generation Subsystem Implementation
// Place in: Source/NeonAscendantCore/Private/Subsystems/

#include "Subsystems/MissionGenerationSubsystem.h"
#include "Subsystems/CodexSubsystem.h"
#include "Data/MissionBrief.h"
#include "Data/GameConstants.h"
#include "Data/ArchetypeData.h"
#include "Data/WeaponData.h"
#include "Data/ImplantData.h"
#include "Data/AbilityData.h"
#include "Data/DistrictData.h"
#include "Data/FactionData.h"

void UMissionGenerationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    CodexSubsystem = GetGameInstance()->GetSubsystem<UCodexSubsystem>();
    if (!CodexSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("MissionGenerationSubsystem: CodexSubsystem not found"));
    }
}

FMissionBrief UMissionGenerationSubsystem::GenerateMissionBrief(int32 Seed)
{
    if (Seed >= 0)
    {
        RandomStream.Initialize(Seed);
    }
    else
    {
        RandomStream.GenerateNewSeed();
    }

    if (!CodexSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("MissionGenerationSubsystem: CodexSubsystem not available"));
        return FMissionBrief();
    }

    FMissionBrief Brief;

    // Choose random district
    TArray<UDistrictDataAsset*> Districts = CodexSubsystem->GetAllDistricts();
    if (Districts.Num() > 0)
    {
        Brief.District = ChooseRandom(Districts);
    }

    // Choose random faction
    TArray<UFactionDataAsset*> Factions = CodexSubsystem->GetAllFactions();
    if (Factions.Num() > 0)
    {
        Brief.Opposition = ChooseRandom(Factions);
    }

    // Choose random archetype
    TArray<UArchetypeDataAsset*> Archetypes = CodexSubsystem->GetAllArchetypes();
    if (Archetypes.Num() > 0)
    {
        Brief.Archetype = ChooseRandom(Archetypes);
    }

    // Choose random weapon
    TArray<UWeaponDataAsset*> Weapons = CodexSubsystem->GetAllWeapons();
    if (Weapons.Num() > 0)
    {
        Brief.PrimaryWeapon = ChooseRandom(Weapons);
    }

    // Choose random implant
    TArray<UImplantDataAsset*> Implants = CodexSubsystem->GetAllImplants();
    if (Implants.Num() > 0)
    {
        Brief.BackupImplant = ChooseRandom(Implants);
    }

    // Choose random ability from archetype
    if (Brief.Archetype.IsValid())
    {
        UArchetypeDataAsset* Archetype = Brief.Archetype.LoadSynchronous();
        if (Archetype && Archetype->Abilities.Num() > 0)
        {
            int32 AbilityIndex = RandomStream.RandRange(0, Archetype->Abilities.Num() - 1);
            Brief.FeaturedAbility = Archetype->Abilities[AbilityIndex];
        }
    }

    // Choose random complication
    Brief.Complication = GetRandomComplication();

    // Choose random extraction condition
    Brief.ExtractionCondition = GetRandomExtractionCondition();

    return Brief;
}

TArray<FMissionBrief> UMissionGenerationSubsystem::GenerateMissionBriefs(int32 Count, int32 Seed)
{
    if (Count <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("MissionGenerationSubsystem: Count must be positive"));
        return TArray<FMissionBrief>();
    }

    TArray<FMissionBrief> Briefs;
    Briefs.Reserve(Count);

    if (Seed >= 0)
    {
        RandomStream.Initialize(Seed);
    }

    for (int32 i = 0; i < Count; i++)
    {
        Briefs.Add(GenerateMissionBrief(-1)); // Use current stream state
    }

    return Briefs;
}

void UMissionGenerationSubsystem::SetSeed(int32 Seed)
{
    RandomStream.Initialize(Seed);
}

template<typename T>
T* UMissionGenerationSubsystem::ChooseRandom(const TArray<T*>& Options)
{
    if (Options.Num() == 0)
    {
        return nullptr;
    }

    int32 Index = RandomStream.RandRange(0, Options.Num() - 1);
    return Options[Index];
}

FString UMissionGenerationSubsystem::GetRandomComplication()
{
    TArray<FString> Complications = UGameConstants::GetComplications();
    if (Complications.Num() == 0)
    {
        return TEXT("No complications available");
    }

    int32 Index = RandomStream.RandRange(0, Complications.Num() - 1);
    return Complications[Index];
}

FString UMissionGenerationSubsystem::GetRandomExtractionCondition()
{
    TArray<FString> Conditions = UGameConstants::GetExtractionConditions();
    if (Conditions.Num() == 0)
    {
        return TEXT("No extraction conditions available");
    }

    int32 Index = RandomStream.RandRange(0, Conditions.Num() - 1);
    return Conditions[Index];
}

// Explicit template instantiations
template UDistrictDataAsset* UMissionGenerationSubsystem::ChooseRandom<UDistrictDataAsset>(const TArray<UDistrictDataAsset*>&);
template UFactionDataAsset* UMissionGenerationSubsystem::ChooseRandom<UFactionDataAsset>(const TArray<UFactionDataAsset*>&);
template UArchetypeDataAsset* UMissionGenerationSubsystem::ChooseRandom<UArchetypeDataAsset>(const TArray<UArchetypeDataAsset*>&);
template UWeaponDataAsset* UMissionGenerationSubsystem::ChooseRandom<UWeaponDataAsset>(const TArray<UWeaponDataAsset*>&);
template UImplantDataAsset* UMissionGenerationSubsystem::ChooseRandom<UImplantDataAsset>(const TArray<UImplantDataAsset*>&);

