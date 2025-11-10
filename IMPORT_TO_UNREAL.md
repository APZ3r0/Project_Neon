# How to Import Unreal Files into Unreal Engine

This guide explains how to copy the template files from this repository into your Unreal Engine 5 project.

## Prerequisites

- Unreal Engine 5.3+ installed
- Visual Studio 2022 with C++ game development tools
- A new or existing Unreal C++ project

## Step-by-Step Import Process

### Step 1: Create Your Unreal Project

1. Open **Unreal Engine 5.3+**
2. Click **Create Project**
3. Choose **C++** (NOT Blueprint-only)
4. Select **Third Person** template
5. Project name: `NeonAscendant` (or your preferred name)
6. Choose a location for your project
7. Click **Create**

> **Important:** The project must be a C++ project, not Blueprint-only, because we're adding C++ code.

### Step 2: Close Unreal Editor

Before adding C++ code, you need to close the Unreal Editor. The editor will automatically reopen after you compile.

### Step 3: Create the Core Module

1. Open your project's `.sln` file in **Visual Studio 2022**
2. In Solution Explorer, right-click your project name (e.g., `NeonAscendant`)
3. Select **Add → New Module**
4. Module name: `NeonAscendantCore`
5. Click **Add**
6. Wait for Visual Studio to generate the module files

This creates:
- `Source/NeonAscendantCore/NeonAscendantCore.Build.cs`
- `Source/NeonAscendantCore/Public/NeonAscendantCore.h`
- `Source/NeonAscendantCore/Private/NeonAscendantCore.cpp`

### Step 4: Copy Build Configuration Files

Copy these files from `unreal_project_setup/` to your Unreal project:

**From:** `unreal_project_setup/NeonAscendantCore.Build.cs`  
**To:** `Source/NeonAscendantCore/NeonAscendantCore.Build.cs`

Replace the auto-generated file with our version (it has the correct dependencies).

### Step 5: Copy Data Structure Headers

Create the folder structure and copy files:

1. Create folder: `Source/NeonAscendantCore/Public/Data/`
2. Copy all files from `unreal_project_setup/Data/` to `Source/NeonAscendantCore/Public/Data/`:
   - `AbilityData.h`
   - `ArchetypeData.h` (from `unreal_templates/`)
   - `DistrictData.h`
   - `FactionData.h`
   - `ImplantData.h`
   - `MissionBrief.h`
   - `WeaponData.h`

### Step 6: Copy Subsystem Files

1. Create folder: `Source/NeonAscendantCore/Public/Subsystems/`
2. Copy from `unreal_templates/`:
   - `CodexSubsystem.h` → `Source/NeonAscendantCore/Public/Subsystems/`
   - `MissionGenerationSubsystem.h` → `Source/NeonAscendantCore/Public/Subsystems/`

3. Create folder: `Source/NeonAscendantCore/Private/Subsystems/`
4. Copy from `unreal_templates/` and `unreal_project_setup/`:
   - `CodexSubsystem.cpp` → `Source/NeonAscendantCore/Private/Subsystems/`
   - `MissionGenerationSubsystem.cpp` → `Source/NeonAscendantCore/Private/Subsystems/`

### Step 7: Copy Constants Header

Copy from `unreal_data/`:
- `GameConstants.h` → `Source/NeonAscendantCore/Public/Data/GameConstants.h`

### Step 8: Update Include Paths

You may need to update some include paths in the copied files. Check for:
- `#include "Data/ArchetypeData.h"` should work if files are in the right folders
- `#include "Subsystems/CodexSubsystem.h"` should work if files are in the right folders

### Step 9: Compile the Project

1. In Visual Studio, right-click the solution
2. Select **Build Solution** (or press `F7`)
3. Wait for compilation to complete
4. If there are errors, fix them (usually missing includes or module names)

### Step 10: Generate Project Files (if needed)

If you get compilation errors about missing files:

1. Close Visual Studio
2. Right-click your `.uproject` file
3. Select **Generate Visual Studio project files**
4. Reopen the solution in Visual Studio
5. Build again

### Step 11: Open Unreal Editor

1. Double-click your `.uproject` file
2. Unreal Editor will open
3. You may see warnings about missing modules - this is normal if you haven't set up the Asset Manager yet

## Quick Copy Script (Windows PowerShell)

You can use this PowerShell script to automate the copying:

```powershell
# Set your Unreal project path
$UnrealProjectPath = "C:\Path\To\Your\Unreal\Project"
$RepoPath = "C:\Path\To\Project_Neon-main"

# Create directories
New-Item -ItemType Directory -Force -Path "$UnrealProjectPath\Source\NeonAscendantCore\Public\Data"
New-Item -ItemType Directory -Force -Path "$UnrealProjectPath\Source\NeonAscendantCore\Public\Subsystems"
New-Item -ItemType Directory -Force -Path "$UnrealProjectPath\Source\NeonAscendantCore\Private\Subsystems"

# Copy files
Copy-Item "$RepoPath\unreal_project_setup\Data\*" -Destination "$UnrealProjectPath\Source\NeonAscendantCore\Public\Data\" -Recurse
Copy-Item "$RepoPath\unreal_templates\ArchetypeData.h" -Destination "$UnrealProjectPath\Source\NeonAscendantCore\Public\Data\"
Copy-Item "$RepoPath\unreal_templates\CodexSubsystem.h" -Destination "$UnrealProjectPath\Source\NeonAscendantCore\Public\Subsystems\"
Copy-Item "$RepoPath\unreal_templates\MissionGenerationSubsystem.h" -Destination "$UnrealProjectPath\Source\NeonAscendantCore\Public\Subsystems\"
Copy-Item "$RepoPath\unreal_templates\CodexSubsystem.cpp" -Destination "$UnrealProjectPath\Source\NeonAscendantCore\Private\Subsystems\"
Copy-Item "$RepoPath\unreal_project_setup\Subsystems\MissionGenerationSubsystem.cpp" -Destination "$UnrealProjectPath\Source\NeonAscendantCore\Private\Subsystems\"
Copy-Item "$RepoPath\unreal_data\GameConstants.h" -Destination "$UnrealProjectPath\Source\NeonAscendantCore\Public\Data\"
Copy-Item "$RepoPath\unreal_project_setup\NeonAscendantCore.Build.cs" -Destination "$UnrealProjectPath\Source\NeonAscendantCore\NeonAscendantCore.Build.cs" -Force

Write-Host "Files copied successfully!"
```

## Importing Data Assets

After the code compiles, you'll need to import the game data:

### Step 1: Import CSV Files as Data Tables

1. In Unreal Editor, go to `Content/Data/` (create if needed)
2. Right-click → **Miscellaneous → Data Table**
3. Choose the appropriate row struct:
   - For `Archetypes.csv` → `FArchetypeTableRow`
   - For `Weapons.csv` → `FWeaponTableRow`
   - For `Districts.csv` → `FDistrictTableRow`
   - For `Factions.csv` → `FFactionTableRow`
4. Click **Import CSV**
5. Select the CSV file from `unreal_data/`
6. Save the Data Table

### Step 2: Create Data Assets

1. Right-click in `Content/Data/Archetypes/`
2. Select **Create → Data Asset → Archetype Data Asset**
3. Name it `DA_Specter`
4. Fill in properties from `unreal_data/codex_export.json`
5. Repeat for all archetypes, weapons, districts, and factions

### Step 3: Configure Asset Manager

1. Edit → Project Settings → **Game → Asset Manager**
2. Add Primary Asset Types:
   - `Archetype`
   - `Weapon`
   - `District`
   - `Faction`
   - `Ability`
   - `Implant`
3. Set Directories for each type to match your Content folder structure

## Troubleshooting

### Compilation Errors

**Error: "Cannot open include file"**
- Check that files are in the correct `Public/` or `Private/` folders
- Verify include paths match the folder structure

**Error: "Unresolved external symbol"**
- Make sure `.cpp` files are in `Private/` folder
- Check that module dependencies are correct in `.Build.cs`

**Error: "Module not found"**
- Regenerate project files (right-click `.uproject` → Generate Visual Studio project files)
- Rebuild solution

### Missing Data

**CodexSubsystem can't find assets:**
- Verify Asset Manager is configured
- Check that Primary Asset Types are set
- Ensure data assets are in the correct Content folders

## Next Steps

After importing:
1. Test the CodexSubsystem in a Blueprint
2. Test MissionGenerationSubsystem
3. Create your first Data Assets
4. See `unreal_project_setup/GETTING_STARTED.md` for more details

## File Structure Reference

Your Unreal project should have this structure after importing:

```
YourUnrealProject/
├── Source/
│   └── NeonAscendantCore/
│       ├── NeonAscendantCore.Build.cs
│       ├── Public/
│       │   ├── NeonAscendantCore.h
│       │   ├── Data/
│       │   │   ├── AbilityData.h
│       │   │   ├── ArchetypeData.h
│       │   │   ├── DistrictData.h
│       │   │   ├── FactionData.h
│       │   │   ├── GameConstants.h
│       │   │   ├── ImplantData.h
│       │   │   ├── MissionBrief.h
│       │   │   └── WeaponData.h
│       │   └── Subsystems/
│       │       ├── CodexSubsystem.h
│       │       └── MissionGenerationSubsystem.h
│       └── Private/
│           ├── NeonAscendantCore.cpp
│           └── Subsystems/
│               ├── CodexSubsystem.cpp
│               └── MissionGenerationSubsystem.cpp
└── Content/
    └── Data/
        ├── Archetypes/
        ├── Weapons/
        ├── Districts/
        └── Factions/
```

---

**Need help?** Check `unreal_project_setup/GETTING_STARTED.md` for more detailed instructions.

