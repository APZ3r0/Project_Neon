# Getting Started - Unreal Engine Migration

## Step 1: Create Unreal Project

1. Open **Unreal Engine 5.3+**
2. Click **Create Project**
3. Choose **C++** (not Blueprint-only)
4. Select **Third Person** template
5. Project name: `NeonAscendant`
6. Location: Choose your workspace
7. Click **Create**

## Step 2: Set Up Project Structure

### Create Folder Structure in Content Browser

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

### Enable Required Plugins

1. Edit → Plugins
2. Search and enable:
   - **Gameplay Ability System** (if available)
   - **Procedural Content Generation Framework** (UE 5.2+)
   - **Online Subsystem**

## Step 3: Create Core Module

1. Close Unreal Editor
2. In Visual Studio, right-click project → **Add → New Module**
3. Module name: `NeonAscendantCore`
4. Click **Add**

## Step 4: Copy Build Files

Copy from `unreal_project_setup/` to your project:

- `NeonAscendantCore.Build.cs` → `Source/NeonAscendantCore/NeonAscendantCore.Build.cs`
- `NeonAscendantCore.h` → `Source/NeonAscendantCore/Public/NeonAscendantCore.h`
- `NeonAscendantCore.cpp` → `Source/NeonAscendantCore/Private/NeonAscendantCore.cpp`

## Step 5: Copy Data Structures

Copy all files from `unreal_project_setup/Data/` to:
`Source/NeonAscendantCore/Public/Data/`

- `ArchetypeData.h` (from `unreal_templates/`)
- `WeaponData.h`
- `DistrictData.h`
- `FactionData.h`
- `AbilityData.h`
- `ImplantData.h`
- `MissionBrief.h`

## Step 6: Copy Subsystems

Copy from `unreal_templates/` and `unreal_project_setup/`:

- `CodexSubsystem.h` → `Source/NeonAscendantCore/Public/Subsystems/`
- `CodexSubsystem.cpp` → `Source/NeonAscendantCore/Private/Subsystems/`
- `MissionGenerationSubsystem.h` → `Source/NeonAscendantCore/Public/Subsystems/`
- `MissionGenerationSubsystem.cpp` → `Source/NeonAscendantCore/Private/Subsystems/`

## Step 7: Copy Constants

Copy `unreal_data/GameConstants.h` to:
`Source/NeonAscendantCore/Public/Data/GameConstants.h`

## Step 8: Update Module References

In `NeonAscendantCore.Build.cs`, ensure all dependencies are listed (should already be correct from template).

## Step 9: Compile Project

1. Right-click `.uproject` file → **Generate Visual Studio project files**
2. Open solution in Visual Studio
3. Build → **Build Solution** (or press F7)
4. Fix any compilation errors

## Step 10: Import Data

### Create Data Tables

1. In Unreal Editor, go to `Content/Data/`
2. Right-click → **Miscellaneous → Data Table**
3. For each CSV file:
   - Choose appropriate row struct (e.g., `FArchetypeTableRow`)
   - Import from `unreal_data/Archetypes.csv`
   - Repeat for Weapons, Districts, Factions

### Create Data Assets

1. Right-click in appropriate folder → **Create → Data Asset**
2. Choose class (e.g., `Archetype Data Asset`)
3. Name: `DA_Specter`
4. Fill in properties from `unreal_data/codex_export.json`
5. Repeat for all archetypes, weapons, districts, factions

## Step 11: Configure Asset Manager

1. Edit → Project Settings → **Game → Asset Manager**
2. Add Primary Asset Types:
   - `Archetype`
   - `Weapon`
   - `District`
   - `Faction`
   - `Ability`
   - `Implant`

3. Set Directories for each type:
   - `/Game/Data/Archetypes`
   - `/Game/Data/Weapons`
   - etc.

## Step 12: Test Codex System

1. Create Blueprint: `BP_TestCodex`
2. Add Event BeginPlay
3. Get Game Instance → Get Subsystem (Codex Subsystem)
4. Call "Get All Archetypes"
5. Print to screen (for testing)

## Step 13: Test Mission Generation

1. Create Blueprint: `BP_TestMissionGen`
2. Add Event BeginPlay
3. Get Game Instance → Get Subsystem (Mission Generation Subsystem)
4. Call "Generate Mission Brief" with seed 42
5. Print mission brief details

## Next Steps

- Implement character system
- Create weapon system
- Set up ability system (GAS)
- Build first playable prototype

## Troubleshooting

### Compilation Errors
- Check all `#include` paths
- Verify module dependencies in `.Build.cs`
- Clean and rebuild solution

### Data Not Loading
- Check Asset Manager configuration
- Verify Primary Asset Types are set
- Ensure data assets are in correct folders

### Subsystem Not Found
- Verify `UGameInstanceSubsystem` inheritance
- Check initialization order
- Ensure module is loaded

---

**Status:** Foundation complete - ready for gameplay implementation!

