# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**Neon Ascendant** is an Unreal Engine 5.7 tactical sci-fi game with a deterministic mission generator. The codebase includes:
- C++ runtime module (`NeonAscendant`) with mission generation, character, and weapon systems
- Blueprint-friendly APIs for all gameplay features
- Core gameplay foundation: player character, weapons, and game mode

**Key Philosophy**: The mission generator is central to the game design. All lore data (districts, factions, archetypes, weapons, implants) is hard-coded in C++ for deterministic, seeded random generation using `FRandomStream`.

## Implementation Status

**Fully Implemented:**
- ✅ Mission generation system with deterministic seeding
- ✅ Player character with dual camera, movement, health, and combat
- ✅ Weapon system with hitscan, ammo, reloading, auto/semi-auto modes
- ✅ Enemy AI with state machine (Patrol, Investigate, Engage, Retreat)
- ✅ Enemy characters with health, weapons, and combat
- ✅ Canvas-based HUD with health bars, ammo counter, mission briefing
- ✅ District hazards with area-of-effect damage and visual effects
- ✅ Complete lore data (districts, factions, archetypes, weapons, implants)

**Not Yet Implemented:**
- ❌ Objective tracking and completion system
- ❌ Extraction zones and win conditions
- ❌ Ability system (referenced in archetype abilities)
- ❌ Progression/rewards system
- ❌ Save/load system
- ❌ Procedural level generation (districts are data-driven but levels are manual)

## Building and Running

### Opening the Project
```bash
# Open in Unreal Engine 5.7
# Double-click NeonAscendant.uproject
# Editor will prompt to rebuild the NeonAscendant module
```

### Compilation
The project uses Visual Studio 2022 or Rider for Unreal Engine:
```bash
# Regenerate project files if needed
# Right-click NeonAscendant.uproject → Generate Visual Studio project files

# Build from Visual Studio
# Open NeonAscendant.sln
# Build → Build Solution (or Ctrl+Shift+B)

# Build from Rider
# Open NeonAscendant.sln in Rider
# Build → Build Solution
```

### Cleaning Build Artifacts
```bash
# Delete build artifacts to force clean rebuild
rm -rf Binaries Intermediate Saved DerivedDataCache .vs

# Regenerate project files
# Right-click NeonAscendant.uproject → Generate Visual Studio project files
```

## Architecture

### Module Structure

The `NeonAscendant` runtime module is organized as follows:

**Public Headers** (`Source/NeonAscendant/Public/`):
- `MissionTypes.h` - All USTRUCT definitions (archetypes, weapons, districts, factions, implants, mission briefs)
- `MissionData.h` - Namespace functions returning lore data arrays
- `MissionGenerator.h` - Blueprint-callable mission generator singleton API
- `NeonCharacter.h` - Player character with first/third-person camera, movement, weapon handling, health system
- `NeonWeapon.h` - Hitscan weapon system with ammo, reloading, auto/semi-auto modes
- `NeonGameMode.h` - Game mode that integrates mission generator
- `NeonEnemy.h` - AI-controlled enemy character with health, weapons, and targeting
- `NeonEnemyController.h` - AI controller with state machine (Patrol, Investigate, Engaged, Retreat)
- `NeonHUD.h` - Canvas-based HUD with health, ammo, mission briefing, and objective tracking
- `DistrictHazard.h` - Environmental hazard actors with damage-over-time effects

**Private Implementation** (`Source/NeonAscendant/Private/`):
- `MissionData.cpp` - **All lore data hard-coded here** (archetypes, weapons, districts, factions, complications, extraction conditions)
- `MissionGenerator.cpp` - Mission assembly logic using `FRandomStream` for deterministic generation
- `NeonCharacter.cpp` - Character movement, input handling, weapon equipping, health and damage
- `NeonWeapon.cpp` - Weapon firing, damage application, ammo management
- `NeonGameMode.cpp` - Game initialization, mission generation startup
- `NeonEnemy.cpp` - Enemy behavior, weapon handling, death logic
- `NeonEnemyController.cpp` - AI state machine implementation, pathfinding, combat logic
- `NeonHUD.cpp` - HUD rendering with Canvas API, mission display, player stats
- `DistrictHazard.cpp` - Hazard overlap detection, damage application, visual effects

**Build Configuration** (`Source/NeonAscendant/NeonAscendant.Build.cs`):
- Public Dependencies: `Core`, `CoreUObject`, `Engine`, `GameplayTags`, `InputCore`, `EnhancedInput`, `AIModule`
- Private Dependencies: `Projects`
- Exceptions enabled (`bEnableExceptions = true`)

### Mission Generation System

The mission generator uses a **singleton pattern** safe for Blueprint access:

```cpp
// Get the generator instance
UMissionGenerator* Gen = UMissionGeneratorSingleton::GetGenerator();

// Seed for deterministic generation (optional)
Gen->SeedGenerator(1337);

// Generate single mission
FMissionBrief Mission = Gen->GenerateMissionBrief();

// Generate multiple missions
TArray<FMissionBrief> Briefs;
Gen->GenerateMissionBriefs(5, Briefs);
```

**Mission Brief Structure**:
- District (location with hazards)
- Opposition faction (tactics and description)
- Archetype (character class with abilities)
- Primary weapon
- Implant (augmentation with effects)
- Complication (difficulty modifier)
- Extraction condition (win condition)

All data is randomly selected from hard-coded arrays in `MissionData.cpp` using `FRandomStream.RandRange()`.

### Gameplay Classes

**ANeonCharacter**:
- Dual camera system (first-person and third-person, toggleable via `bUseFirstPerson`)
- Movement: WASD, jump, crouch (with speed multipliers), sprint
- Input: Mouse look, gamepad support
- Weapon system: Equips weapon via `EquipWeapon()`, fires via input bindings
- Health system: `MaxHealth`, `CurrentHealth`, `TakeDamage()`, `Die()`, health percentage getters
- Blueprint-callable: `GetHealth()`, `GetMaxHealth()`, `GetHealthPercent()`
- Default walk speed: 600.0, sprint multiplier: 1.5x, crouch multiplier: 0.5x

**ANeonWeapon**:
- Hitscan shooting using line traces from `MuzzleLocation` arrow component
- Fire modes: Automatic (`bIsAutomatic = true`) or semi-automatic
- Ammo system: `CurrentAmmo`, `MaxAmmo`, reload timer
- Damage application to hit actors implementing `TakeDamage()`
- Blueprint-callable: `StartFire()`, `StopFire()`, `Reload()`, `GetCurrentAmmo()`, `IsReloading()`

**ANeonGameMode**:
- Sets up player character on `BeginPlay()`
- Integrates mission generator via `GetMissionGenerator()`
- `StartNewMission()` generates new mission brief

**ANeonEnemy**:
- AI-controlled enemy character inheriting from `ACharacter`
- Health system: `MaxHealth`, `CurrentHealth`, `TakeDamage()`, `Die()`
- Weapon system: `WeaponClass`, `EquippedWeapon`, `FireWeapon()`
- Detection and targeting: `DetectionRange`, `AttackRange`, `TargetPlayer`
- Combat configuration: `FireInterval` for controlling rate of fire
- Blueprint-callable: `GetEnemyController()`, exposes `bIsDead` state
- Automatically controlled by `ANeonEnemyController` AI

**ANeonEnemyController**:
- AI controller with state machine: `Patrol`, `Investigate`, `Engaged`, `Retreat`, `Dead`
- Perception system: Line-of-sight checks, damage awareness, player tracking
- Patrol behavior: Random waypoint generation, configurable speed and wait times
- Investigation: Searches last known player location when target lost
- Engagement: Moves to attack range and commands enemy to fire weapon
- Retreat: Falls back when health drops below threshold (default 25%)
- Blueprint-accessible: `GetAIState()`, state change events
- Configurable ranges: `DetectionRange`, `AttackRange`, `LostTargetDistance`

**ANeonHUD**:
- Canvas-based HUD inheriting from `AHUD`
- Displays health bar with color-coded status (green/yellow/red)
- Shows current mission briefing (district, opposition, archetype, weapon, etc.)
- Ammo counter synchronized with equipped weapon
- Objective tracker for current mission goals
- Extraction zone indicator
- Blueprint-callable: `SetMissionBrief()`, `SetPlayerCharacter()`
- Fully customizable: colors, positions, sizes all exposed as Blueprint properties
- HUD layout configurable via properties: `HealthBarPosition`, `MissionBriefingPosition`, etc.

**ADistrictHazard**:
- Environmental hazard actor causing area-of-effect damage
- Hazard types: `Thermal`, `Electrical`, `Toxic`, `Radiation`, `Cryogenic`
- Damage-over-time: Configurable `DamagePerSecond` and `DamageTickRate`
- Sphere-based area of effect with configurable `EffectRadius`
- Automatic overlap detection and damage application to characters
- Visual feedback: `HazardColor`, `VisualIntensity`, particle effects
- Blueprint-configurable: All damage and visual parameters exposed
- Can be toggled on/off via `bIsActive` property
- Corresponds to hazard arrays in mission district data

### Input Bindings

Configured in `Config/DefaultInput.ini`:
- W/A/S/D - Movement
- Mouse - Look
- Space - Jump
- Left Shift - Sprint
- Left Ctrl / C - Crouch
- Left Mouse Button - Fire
- R - Reload

## Development Workflows

### Adding New Lore Data

**New Archetype**:
1. Add `FAscendantArchetype` entry to `MissionData.cpp::NeonAscendantData::GetArchetypes()`
2. Include name, role, signature, and abilities array
3. Recompile module
4. New archetype auto-included in random selection

**New Weapon/Implant/District/Faction**:
1. Add entry to corresponding function in `MissionData.cpp` (`GetWeapons()`, `GetImplants()`, etc.)
2. Follow existing struct pattern from `MissionTypes.h`
3. Recompile

**New Complication or Extraction Condition**:
1. Add string to `GetComplications()` or `GetExtractionConditions()` array in `MissionData.cpp`
2. Recompile

### Creating Blueprints

**Weapon Blueprint**:
1. Content Browser → Blueprint Class → `NeonWeapon`
2. Set properties: `Damage`, `FireRate`, `MaxAmmo`, `Range`, `ReloadTime`, `bIsAutomatic`
3. Assign `WeaponMesh` skeletal mesh and material

**Character Blueprint**:
1. Content Browser → Blueprint Class → `NeonCharacter`
2. Set `StartingWeaponClass` to your weapon Blueprint
3. Configure `bUseFirstPerson`, `SprintSpeedMultiplier`, `CrouchSpeedMultiplier`
4. Assign skeletal mesh

**Game Mode Blueprint**:
1. Content Browser → Blueprint Class → `NeonGameMode`
2. Set `Default Pawn Class` to your character Blueprint
3. Use `StartNewMission()` to generate missions on level start

**Enemy Blueprint**:
1. Content Browser → Blueprint Class → `NeonEnemy`
2. Set `WeaponClass` to the weapon Blueprint you want the enemy to use
3. Configure `MaxHealth`, `DetectionRange`, `AttackRange`, `FireInterval`
4. Assign skeletal mesh and animations
5. Place in level - will automatically use AI controller for behavior

**HUD Blueprint**:
1. Content Browser → Blueprint Class → `NeonHUD`
2. Configure colors: `HealthyColor`, `DamagedColor`, `CriticalColor`, `TextColor`
3. Adjust layout positions: `HealthBarPosition`, `AmmoCounterPosition`, etc.
4. Set in Game Mode's HUD Class property
5. HUD will automatically track player and display mission info

**District Hazard Blueprint**:
1. Content Browser → Blueprint Class → `DistrictHazard`
2. Set `HazardType` (Thermal, Electrical, Toxic, Radiation, Cryogenic)
3. Configure `DamagePerSecond`, `EffectRadius`, `DamageTickRate`
4. Set `HazardColor` and `VisualIntensity` for visual feedback
5. Place in level to create danger zones matching district theme

### Debugging Mission Generation

Use deterministic seeding to reproduce specific missions:
```cpp
// In Blueprint or C++
UMissionGenerator* Gen = UMissionGeneratorSingleton::GetGenerator();
Gen->SeedGenerator(1337);  // Always generates same sequence
FMissionBrief Mission = Gen->GenerateMissionBrief();

// Print mission details to log
UE_LOG(LogTemp, Warning, TEXT("District: %s"), *Mission.District.Name);
UE_LOG(LogTemp, Warning, TEXT("Opposition: %s"), *Mission.Opposition.Name);
```

## Key Conventions

### Blueprint Exposure
- All gameplay classes use `UCLASS(BlueprintType)` or `UCLASS(Blueprintable)`
- Properties exposed with `UPROPERTY(BlueprintReadOnly)` or `UPROPERTY(BlueprintReadWrite)`
- Functions exposed with `UFUNCTION(BlueprintCallable)` or `UFUNCTION(BlueprintPure)`
- Categories used consistently: `"Weapon"`, `"Movement"`, `"Camera"`, `"Mission"`

### Random Generation
- **Always use `FRandomStream`**, never global `rand()` or `FMath::Rand()`
- Seed before generating: `RandomStream.Initialize(Seed)` or `SeedGenerator(Seed)`
- Index selection: `RandomStream.RandRange(0, Array.Num() - 1)`
- Pass stream by reference to helper functions

### Naming Patterns
- Classes: `ANeon*` for Actors, `UMission*` for UObjects
- Blueprint properties: Category tags group related properties
- Hard-coded data: Static const arrays in `NeonAscendantData` namespace

### Data Organization
- All lore data in `MissionData.cpp`, accessed via namespace functions
- No data tables or external JSON - everything is C++ for deterministic compilation
- Static const arrays initialized once and cached
- Blueprint-facing structs defined in `MissionTypes.h`

## Common Patterns

### Spawning Enemies
```cpp
// In level initialization or game mode
void AMyGameMode::SpawnEnemies(const FMissionBrief& Mission)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Spawn enemy at location
    ANeonEnemy* Enemy = GetWorld()->SpawnActor<ANeonEnemy>(
        EnemyClass,
        SpawnLocation,
        FRotator::ZeroRotator,
        SpawnParams
    );

    // Configure based on mission opposition
    Enemy->MaxHealth = CalculateHealthFromFaction(Mission.Opposition);
    Enemy->WeaponClass = GetWeaponFromMissionData(Mission.PrimaryWeapon);
}
```

### Setting Up HUD
```cpp
// In game mode or player controller
void AMyGameMode::InitializeHUD(ANeonCharacter* Player, const FMissionBrief& Mission)
{
    if (ANeonHUD* HUD = Cast<ANeonHUD>(Player->GetController()->GetHUD()))
    {
        HUD->SetPlayerCharacter(Player);
        HUD->SetMissionBrief(Mission);
    }
}
```

### Placing District Hazards
```cpp
// In level setup or procedural generation
void AMyLevelGenerator::PlaceHazards(const FAscendantDistrict& District)
{
    for (const FString& HazardName : District.Hazards)
    {
        EHazardType Type = GetHazardTypeFromName(HazardName);

        FActorSpawnParameters SpawnParams;
        ADistrictHazard* Hazard = GetWorld()->SpawnActor<ADistrictHazard>(
            DistrictHazardClass,
            GetRandomLocationInDistrict(),
            FRotator::ZeroRotator,
            SpawnParams
        );

        Hazard->HazardType = Type;
        Hazard->DamagePerSecond = 15.0f;
        Hazard->EffectRadius = 500.0f;
    }
}
```

### Customizing AI Behavior
```cpp
// Access AI state in gameplay code
void AMyActor::ReactToEnemyState(ANeonEnemy* Enemy)
{
    ANeonEnemyController* AIController = Enemy->GetEnemyController();
    if (AIController)
    {
        EEnemyAIState State = AIController->GetAIState();
        switch (State)
        {
            case EEnemyAIState::Patrol:
                // Enemy is patrolling, unaware
                break;
            case EEnemyAIState::Investigate:
                // Enemy is searching for player
                break;
            case EEnemyAIState::Engaged:
                // Enemy is actively fighting
                break;
            case EEnemyAIState::Retreat:
                // Enemy is retreating (low health)
                break;
        }
    }
}
```

### Getting Mission Data in C++
```cpp
#include "MissionGenerator.h"

void AMyActor::GenerateMissions()
{
    UMissionGenerator* Generator = UMissionGeneratorSingleton::GetGenerator();
    Generator->SeedGenerator(FMath::Rand()); // or fixed seed for determinism

    TArray<FMissionBrief> Briefs;
    Generator->GenerateMissionBriefs(10, Briefs);

    for (const FMissionBrief& Brief : Briefs)
    {
        // Use Brief.District, Brief.Opposition, etc.
    }
}
```

### Spawning Weapons
```cpp
// In character class
void ANeonCharacter::EquipWeapon(TSubclassOf<ANeonWeapon> WeaponClass)
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Destroy();
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = this;

    CurrentWeapon = GetWorld()->SpawnActor<ANeonWeapon>(WeaponClass, SpawnParams);
    CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
}
```

### Accessing Weapon State in Blueprint
```blueprint
Get Current Weapon → Get Current Ammo
Get Current Weapon → Is Reloading
```

### Accessing Health in Blueprint
```blueprint
// Player character health
Get Health Percent → [0.0 to 1.0 for UI bars]
Get Health → [Current health value]
Get Max Health → [Maximum health value]

// Enemy health
Cast to NeonEnemy → Get Current Health
Cast to NeonEnemy → Is Dead
```

### Checking AI State in Blueprint
```blueprint
Get Enemy Controller → Get AI State → Switch on EEnemyAIState
  - Patrol: Do something
  - Engaged: Do something else
  - Retreat: Handle retreat
```

## Important Notes

### Module Dependencies
The `NeonAscendant` module requires:
- `GameplayTags` - for archetype identification
- `EnhancedInput` - for improved input handling
- `InputCore` - for legacy input binding support
- `AIModule` - for enemy AI controllers and navigation

If adding new systems, update `NeonAscendant.Build.cs` with required modules.

### Blueprint vs C++
- Prototype gameplay in Blueprints first
- Move performance-critical code to C++ later
- Character/weapon stats are designed to be tweaked in Blueprint child classes
- Mission generation logic stays in C++ for determinism

### Version Control
- Project uses Git with GitHub remote
- Visual Studio solution file (`.sln`) and binaries are in `.gitignore`
- Content assets tracked via Git LFS (large file storage)

### Platform Targets
- Primary: Windows 64-bit (`Win64`)
- Secondary: Linux
- Uses Visual Studio Tools plugin for Windows development

## Extending the System

### Adding Abilities
1. Create `UNeonAbility` base class inheriting from `UObject`
2. Implement cooldown system using timers
3. Add ability instances to `ANeonCharacter` and `ANeonEnemy`
4. Bind to input actions for player, trigger via AI for enemies
5. Reference archetype abilities from mission brief for flavor text

### Enhancing Enemy AI
The enemy AI system is fully implemented with a state machine. To enhance it:
1. Add new states to `EEnemyAIState` enum in `NeonEnemyController.h`
2. Implement state behavior in new `Update[StateName]Behavior()` methods
3. Add state transitions in `UpdateAIBehavior()` based on conditions
4. Configure behavior parameters via Blueprint child classes
5. Consider adding Behavior Trees for more complex decision-making

### Building Districts
1. Create level geometry matching district themes (Neon Sprawl, Chrome Heights, etc.)
2. Place `ADistrictHazard` actors matching `FAscendantDistrict.Hazards` array
3. Configure hazard types and damage based on district theme
4. Place enemy spawn points using mission's Opposition faction
5. Add Navigation Mesh Bounds Volume for AI pathfinding
6. Configure lighting and atmosphere using Lumen for cyberpunk aesthetic
7. Use district data to determine enemy density and weapon types

### Enhancing the HUD
The Canvas-based HUD is implemented. To customize or extend:
1. Subclass `ANeonHUD` in Blueprint or C++
2. Override `DrawHUD()` to add custom Canvas drawing
3. Add UMG widgets for more complex UI elements (inventory, skill trees, etc.)
4. Use `SetMissionBrief()` to update mission display dynamically
5. Add minimap, compass, or waypoint markers using Canvas API
6. Implement damage indicators, crosshairs, or hit markers
7. Consider hybrid approach: Canvas for performance-critical elements, UMG for menus

### Implementing Objectives and Extraction
1. Create objective system tracking mission goals
2. Add extraction zone actors that check win conditions from `FMissionBrief.ExtractionCondition`
3. Track kill counts, time limits, or item collection based on mission type
4. Update HUD objective tracker with current progress
5. Trigger mission complete when extraction condition met
6. Award rewards based on mission difficulty (complications, district hazards)
