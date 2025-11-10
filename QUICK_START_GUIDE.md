# Quick Start Guide: Unreal Migration

This guide provides step-by-step instructions to get started with the Unreal Engine migration.

## Prerequisites

1. **Unreal Engine 5.3+** installed
2. **Visual Studio 2022** with C++ game development tools
3. **Git LFS** (for version control of large assets)
4. Basic familiarity with Unreal Engine and C++

## Step 1: Project Setup (Day 1)

### Create New Project

1. Open Unreal Engine
2. Create new **C++** project (not Blueprint-only)
3. Choose **Third Person** template
4. Project name: `NeonAscendant`
5. Location: Choose your workspace

### Project Settings

1. Edit → Project Settings → Engine → General Settings
   - Default Map: Create new empty map
   - Editor Startup Map: Same empty map
2. Edit → Project Settings → Engine → Input
   - Keep default input mappings (we'll customize later)

### Enable Required Plugins

1. Edit → Plugins
2. Enable:
   - **Gameplay Ability System** (if available)
   - **Procedural Content Generation Framework** (UE 5.2+)
   - **Online Subsystem** (for multiplayer)

## Step 2: Project Structure (Day 1-2)

### Create Folder Structure

In Content Browser, create:

```
Content/
├── Data/
│   ├── Archetypes/
│   ├── Abilities/
│   ├── Weapons/
│   ├── Implants/
│   ├── Districts/
│   └── Factions/
├── Blueprints/
│   ├── Characters/
│   ├── Weapons/
│   ├── Abilities/
│   └── GameModes/
├── Maps/
│   ├── Districts/
│   └── Hideout/
└── Materials/
```

### Create C++ Modules

1. Close Unreal Editor
2. In Visual Studio, right-click project → Add → New Module
3. Create modules:
   - `NeonAscendantCore` (foundation)
   - `NeonAscendantGameplay` (combat, characters)
   - `NeonAscendantLoot` (loot system)
   - `NeonAscendantProcedural` (generation)

### Update Build Files

Edit `NeonAscendant.Build.cs`:

```csharp
PublicDependencyModuleNames.AddRange(new string[] {
    "Core",
    "CoreUObject",
    "Engine",
    "InputCore",
    "GameplayAbilities",  // If using GAS
    "ProceduralContentGeneration",  // If using PCG
    "OnlineSubsystem",
    "UMG",  // For UI
});
```

## Step 3: Port Data Models (Day 2-3)

### Create Core Structs

Create `Source/NeonAscendantCore/Public/Data/ArchetypeData.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArchetypeData.generated.h"

USTRUCT(BlueprintType)
struct FArchetypeProfile
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Stealth = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Assault = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Tech = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Resilience = 0;
};

UCLASS(BlueprintType)
class NEONASCENDANTCORE_API UArchetypeDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString ArchetypeName;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Role;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Signature;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FArchetypeProfile BaseStats;
};
```

### Create Codex Subsystem

Create `Source/NeonAscendantCore/Public/Subsystems/CodexSubsystem.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CodexSubsystem.generated.h"

class UArchetypeDataAsset;

UCLASS()
class NEONASCENDANTCORE_API UCodexSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
    
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    UFUNCTION(BlueprintCallable)
    TArray<UArchetypeDataAsset*> GetAllArchetypes() const;
    
    UFUNCTION(BlueprintCallable)
    UArchetypeDataAsset* GetArchetypeByName(const FString& Name) const;
    
private:
    UPROPERTY()
    TArray<UArchetypeDataAsset*> Archetypes;
    
    void LoadArchetypes();
};
```

## Step 4: Export Python Data (Day 3)

### Create Export Script

Create `scripts/export_to_unreal.py`:

```python
"""Export Python codex data to JSON for Unreal import."""

import json
from pathlib import Path
import sys

# Add src to path
sys.path.insert(0, str(Path(__file__).parent.parent / "src"))

from neon_ascendant.data import build_codex

def export_codex():
    codex = build_codex()
    
    export_data = {
        "archetypes": [
            {
                "name": arch.name,
                "role": arch.role,
                "signature": arch.signature,
                "stats": {
                    "stealth": 0,  # Will be filled from simulation.py
                    "assault": 0,
                    "tech": 0,
                    "resilience": 0
                },
                "abilities": [a.name for a in arch.abilities]
            }
            for arch in codex["archetypes"]
        ],
        "weapons": [
            {
                "name": w.name,
                "category": w.category,
                "description": w.description,
                "damage_profile": w.damage_profile
            }
            for w in codex["weapons"]
        ],
        "districts": [
            {
                "name": d.name,
                "description": d.description,
                "hazards": d.hazards,
                "enemy_profiles": d.enemy_profiles
            }
            for d in codex["districts"]
        ],
        "factions": [
            {
                "name": f.name,
                "philosophy": f.philosophy,
                "signature_tactics": f.signature_tactics
            }
            for f in codex["factions"]
        ]
    }
    
    output_path = Path(__file__).parent.parent / "unreal_data" / "codex_export.json"
    output_path.parent.mkdir(exist_ok=True)
    
    with open(output_path, "w", encoding="utf-8") as f:
        json.dump(export_data, f, indent=2, ensure_ascii=False)
    
    print(f"Exported codex to {output_path}")

if __name__ == "__main__":
    export_codex()
```

Run: `python scripts/export_to_unreal.py`

## Step 5: Create Data Assets (Day 4)

### Import Archetype Data

1. In Unreal Editor, go to `Content/Data/Archetypes/`
2. Right-click → Create → Data Asset → Archetype Data Asset
3. Name: `DA_Specter`
4. Set properties:
   - Archetype Name: "Specter"
   - Role: "Stealth infiltrator"
   - Signature: "Adaptive camouflage and ghosting protocols"
   - Base Stats: Stealth=9, Assault=5, Tech=8, Resilience=4
5. Repeat for other archetypes

### Create Data Table (Alternative)

1. Create CSV file from exported JSON
2. In Unreal: Right-click → Miscellaneous → Data Table
3. Choose row struct (create `FArchetypeTableRow`)
4. Import CSV

## Step 6: Test Data Access (Day 4)

### Create Test Blueprint

1. Create Blueprint: `BP_TestCodex`
2. Add Event BeginPlay node
3. Get Game Instance → Get Subsystem (Codex Subsystem)
4. Call "Get All Archetypes"
5. Print to screen (for testing)

### Test in C++

Create simple test function in CodexSubsystem:

```cpp
void UCodexSubsystem::LoadArchetypes()
{
    // Load all archetype data assets
    // This is a placeholder - implement actual loading
    UE_LOG(LogTemp, Warning, TEXT("Loading archetypes..."));
}
```

## Step 7: Port Mission Generator (Day 5-6)

### Create Mission Generation Subsystem

Follow the pattern from `PYTHON_TO_UNREAL_MAPPING.md` to create:
- `UMissionGenerationSubsystem`
- `FMissionBrief` struct
- Random selection functions

### Test Mission Generation

1. Create test Blueprint that calls `GenerateMissionBrief`
2. Print mission brief details to screen
3. Verify it matches Python output (with same seed)

## Step 8: Basic Character Setup (Day 7)

### Create Character Class

1. Create C++ class: `ANeonAscendantCharacter` (inherit from `ACharacter`)
2. Add archetype reference
3. Add stat system (Stealth, Assault, Tech, Resilience)
4. Compile and create Blueprint child

### Test Character

1. Place character in test level
2. Verify movement works
3. Test archetype selection

## Next Steps

After completing the quick start:

1. **Week 2**: Implement weapon system
2. **Week 3**: Implement ability system (GAS)
3. **Week 4**: Basic combat loop
4. Continue with full migration plan phases

## Common Issues & Solutions

### Issue: Module not found
**Solution**: Regenerate project files (Right-click `.uproject` → Generate Visual Studio project files)

### Issue: Data assets not loading
**Solution**: Ensure data assets are in correct folder, check `FPrimaryAssetId` setup

### Issue: Subsystem not accessible
**Solution**: Verify `UGameInstanceSubsystem` inheritance, check initialization order

### Issue: Compilation errors
**Solution**: 
- Check `#include` statements
- Verify module dependencies in `.Build.cs`
- Clean and rebuild solution

## Resources

- **Unreal Documentation**: https://docs.unrealengine.com/
- **Gameplay Ability System**: https://github.com/tranek/GASDocumentation
- **C++ Programming Guide**: https://docs.unrealengine.com/en-US/programming-and-scripting/programming-with-cpp/
- **Data Assets**: https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/Assets/DataAssets/

## Getting Help

1. Check `UNREAL_MIGRATION_PLAN.md` for architecture details
2. Check `PYTHON_TO_UNREAL_MAPPING.md` for code translations
3. Unreal Engine forums: https://forums.unrealengine.com/
4. Discord: (create team Discord server)

---

**Estimated Time**: 1 week for quick start (Days 1-7)
**Prerequisites**: Basic Unreal/C++ knowledge
**Outcome**: Working data system and basic character setup

