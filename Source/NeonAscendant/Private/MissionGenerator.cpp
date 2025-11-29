#include "MissionGenerator.h"

#include "MissionData.h"
#include "Misc/DateTime.h"

UMissionGenerator::UMissionGenerator()
    : bHasSeed(false)
{
}

void UMissionGenerator::SeedGenerator(int32 Seed)
{
    RandomStream.Initialize(Seed);
    bHasSeed = true;
}

void UMissionGenerator::EnsureRandomStream()
{
    if (!bHasSeed)
    {
        const int32 Seed = static_cast<int32>(FDateTime::UtcNow().GetTicks() & 0xFFFFFFFF);
        RandomStream.Initialize(Seed);
        bHasSeed = true;
    }
}

FMissionBrief UMissionGenerator::GenerateMissionBrief()
{
    EnsureRandomStream();

    const TArray<FAscendantDistrict>& Districts = NeonAscendantData::GetDistricts();
    const TArray<FAscendantFaction>& Factions = NeonAscendantData::GetFactions();
    const TArray<FAscendantArchetype>& Archetypes = NeonAscendantData::GetArchetypes();
    const TArray<FAscendantWeapon>& Weapons = NeonAscendantData::GetWeapons();
    const TArray<FAscendantImplant>& Implants = NeonAscendantData::GetImplants();
    const TArray<FString>& Complications = NeonAscendantData::GetComplications();
    const TArray<FString>& ExtractionConditions = NeonAscendantData::GetExtractionConditions();

    const FAscendantDistrict& District = Districts[RandomStream.RandRange(0, Districts.Num() - 1)];
    const FAscendantFaction& Opposition = Factions[RandomStream.RandRange(0, Factions.Num() - 1)];
    const FAscendantArchetype& Archetype = Archetypes[RandomStream.RandRange(0, Archetypes.Num() - 1)];
    const FAscendantWeapon& Weapon = Weapons[RandomStream.RandRange(0, Weapons.Num() - 1)];
    const FAscendantImplant& Implant = Implants[RandomStream.RandRange(0, Implants.Num() - 1)];
    const FAscendantAbility& Ability = Archetype.SignatureAbilities[RandomStream.RandRange(0, Archetype.SignatureAbilities.Num() - 1)];
    const FString& Complication = Complications[RandomStream.RandRange(0, Complications.Num() - 1)];
    const FString& ExtractionCondition = ExtractionConditions[RandomStream.RandRange(0, ExtractionConditions.Num() - 1)];

    FMissionBrief Brief;
    Brief.District = District;
    Brief.Opposition = Opposition;
    Brief.Archetype = Archetype;
    Brief.PrimaryWeapon = Weapon;
    Brief.BackupImplant = Implant;
    Brief.FeaturedAbility = Ability;
    Brief.Complication = Complication;
    Brief.ExtractionCondition = ExtractionCondition;

    return Brief;
}

void UMissionGenerator::GenerateMissionBriefs(int32 Count, TArray<FMissionBrief>& OutBriefs)
{
    ensureMsgf(Count > 0, TEXT("GenerateMissionBriefs requires Count to be positive."));

    OutBriefs.Reset();
    OutBriefs.Reserve(Count);

    for (int32 Index = 0; Index < Count; ++Index)
    {
        OutBriefs.Add(GenerateMissionBrief());
    }
}

TObjectPtr<UMissionGenerator> UMissionGeneratorSingleton::GeneratorInstance = nullptr;

UMissionGenerator* UMissionGeneratorSingleton::GetGenerator()
{
    if (!GeneratorInstance)
    {
        GeneratorInstance = NewObject<UMissionGenerator>();
        GeneratorInstance->AddToRoot();
    }

    return GeneratorInstance;
}
