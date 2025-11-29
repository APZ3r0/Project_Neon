#pragma once

#include "CoreMinimal.h"
#include "MissionTypes.h"

namespace NeonAscendantData
{
    const TArray<FAscendantArchetype>& GetArchetypes();
    const TArray<FAscendantWeapon>& GetWeapons();
    const TArray<FAscendantImplant>& GetImplants();
    const TArray<FAscendantDistrict>& GetDistricts();
    const TArray<FAscendantFaction>& GetFactions();
    const TArray<FString>& GetComplications();
    const TArray<FString>& GetExtractionConditions();
}
