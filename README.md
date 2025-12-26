# Project_Neon

## Neon Ascendant – Unreal Engine 5 Complete Tactical Mission Game

**Status:** ✅ **COMPLETE GAMEPLAY IMPLEMENTATION**

The **Neon Ascendant** initiative has been migrated from the original Python lore tools into a **fully playable Unreal Engine 5 game** with procedurally generated missions, intelligent enemy AI, environmental hazards, and a complete HUD/UI system.

### What's Implemented

✅ **Mission Generator** - Deterministic, seeded missions with faction/district/weapon/archetype data  
✅ **Enemy AI System** - State machine with patrol, investigation, combat, and retreat behaviors  
✅ **Combat System** - Bilateral weapon fire between player and enemies  
✅ **Environmental Hazards** - 5 themed hazard types matching districts (Thermal, Electrical, Toxic, Radiation, Cryogenic)  
✅ **Health & Death** - Player and enemy damage systems with ragdoll physics  
✅ **HUD/UI** - Real-time display of health, ammo, mission briefing, and objectives  

### Quick Start

See [BLUEPRINT_SETUP_GUIDE.md](BLUEPRINT_SETUP_GUIDE.md) for step-by-step setup instructions.

**TL;DR:**
1. Compile C++ code (Visual Studio)
2. Create `BP_NeonEnemy` blueprint (parent: ANeonEnemy, set WeaponClass)
3. Create `BP_NeonGameMode` blueprint (parent: ANeonGameMode, set Enemy Class)
4. Open level, set World Settings → Game Mode to BP_NeonGameMode
5. Build NavMesh
6. Press Play

---

## Gameplay Features

### Mission Generation
- Random seeded missions using `FRandomStream`
- Mission includes: District, Opposition faction, Player archetype, Weapon, Complication, Extraction condition
- Deterministic: Same seed generates same mission every time
- **API:** `UMissionGenerator::GenerateMissionBrief()`

### Enemy AI
- **5-State Machine:** Patrol → Investigate → Engaged → Retreat → Dead
- **Intelligent Behavior:**
  - Patrol with random waypoints
  - Investigate when taking damage
  - Chase player with line-of-sight validation
  - Retreat when health below 25%
  - Autonomous weapon firing with inaccuracy
- **Configurable:** Detection range, fire rate, movement speed, health thresholds

### Combat
- **Hitscan Shooting:** Line traces detect hits between player and enemies
- **Weapon Integration:** Configurable damage, fire rate, ammo system
- **Autonomous Enemy Fire:** Enemies aim and shoot at player with 20% inaccuracy
- **Health Tracking:** Both player and enemies take damage, die with ragdoll

### Environmental Hazards
- **5 Hazard Types:** Thermal (Red), Electrical (Yellow), Toxic (Green), Radiation (Cyan), Cryogenic (Blue)
- **Dynamic Spawning:** 2-3 random hazards per mission with varying damage/radius
- **Mission-Driven:** Hazard types can be themed to match district complications
- **Damage System:** Overlapping characters take damage at configurable intervals

### HUD/UI System
- **Health Bar:** Top-left with color coding (Green → Yellow → Red)
- **Ammo Counter:** Bottom-left, linked to equipped weapon
- **Mission Briefing:** Full briefing showing all mission details
- **Objective Tracker:** Shows current complication and extraction method
- **Extraction Indicator:** Top-right status display
- **Canvas-Based:** Responsive, extensible rendering system

---

## Repository Layout

```
Config/                         Project configuration defaults
NeonAscendant.uproject          Unreal project descriptor
README.md                       This file
BLUEPRINT_SETUP_GUIDE.md        How to create blueprints in editor
COMPLETE_GAMEPLAY_IMPLEMENTATION.md  Technical deep dive
IMPLEMENTATION_COMPLETE.md      Summary of all features
GAME_DEVELOPMENT.md             Development roadmap (updated)
ENEMY_AI_INTEGRATION.md         AI system architecture
Source/NeonAscendant/           Runtime module with gameplay code
  ├── Public/
  │     ├── MissionGenerator.h      Mission generation API
  │     ├── MissionTypes.h          Data structures (FMissionBrief, etc.)
  │     ├── NeonCharacter.h         Player character
  │     ├── NeonWeapon.h            Weapon system
  │     ├── NeonGameMode.h          Game mode (NEW)
  │     ├── NeonEnemy.h             Enemy character (NEW)
  │     ├── NeonEnemyController.h   Enemy AI (NEW)
  │     ├── DistrictHazard.h        Hazard system (NEW)
  │     └── NeonHUD.h               HUD/UI system (NEW)
  └── Private/
        ├── MissionGenerator.cpp    Mission logic
        ├── MissionData.cpp         All lore data (archetypes, weapons, etc.)
        ├── NeonCharacter.cpp       Player implementation (updated)
        ├── NeonGameMode.cpp        Game mode with mission orchestration (NEW)
        ├── NeonEnemy.cpp           Enemy implementation (NEW)
        ├── NeonEnemyController.cpp Enemy AI logic (NEW)
        ├── DistrictHazard.cpp      Hazard implementation (NEW)
        └── NeonHUD.cpp             HUD rendering (NEW)
```

---

## Core Systems

### 1. Mission Generator
**Files:** `MissionGenerator.h/cpp`  
**Purpose:** Generate random, deterministic tactical missions  
**API:**
```cpp
UMissionGenerator* Gen = UMissionGeneratorSingleton::GetGenerator();
Gen->SeedGenerator(1337);  // Deterministic seed
FMissionBrief Mission = Gen->GenerateMissionBrief();
```

### 2. Game Mode (Orchestrator)
**Files:** `NeonGameMode.h/cpp`  
**Purpose:** Coordinate mission generation, enemy spawning, hazard placement, HUD updates  
**Key Functions:**
- `StartNewMission()` - Generate mission and configure level
- `SpawnEnemiesForMission()` - Create enemies from mission data
- `SpawnHazardsForMission()` - Create environmental hazards

### 3. Enemy AI
**Files:** `NeonEnemy.h/cpp` + `NeonEnemyController.h/cpp`  
**Purpose:** Intelligent autonomous enemies with tactical behavior  
**States:** Patrol, Investigate, Engaged, Retreat, Dead  
**Features:** Patrol with waypoints, chase with line-of-sight, retreat when damaged, autonomous firing

### 4. Hazard System
**Files:** `DistrictHazard.h/cpp`  
**Purpose:** Environmental damage zones matching district themes  
**Types:** 5 themed hazard types with configurable damage and radius  
**Integration:** Spawned per mission, damages both player and enemies

### 5. HUD/UI
**Files:** `NeonHUD.h/cpp`  
**Purpose:** Display all game information to player  
**Elements:** Health bar, ammo counter, mission briefing, objectives, extraction status  
**Rendering:** Canvas-based, responsive, extensible

---

## Architecture

```
                    ┌──────────────────────────┐
                    │   ANeonGameMode          │
                    │  (Mission Orchestrator)  │
                    └──────────────────────────┘
                              │
        ┌─────────────────────┼──────────────────────┐
        ▼                     ▼                      ▼
   ┌─────────────┐    ┌──────────────┐      ┌──────────────┐
   │  Enemies    │    │   Hazards    │      │    HUD       │
   │  (AI-driven)│    │  (Physics)   │      │  (Canvas)    │
   ├─────────────┤    ├──────────────┤      ├──────────────┤
   │ Patrol      │    │ Thermal      │      │ Health Bar   │
   │ Chase       │    │ Electrical   │      │ Ammo Counter │
   │ Retreat     │    │ Toxic        │      │ Briefing     │
   │ Fire        │    │ Radiation    │      │ Objectives   │
   └─────────────┘    │ Cryogenic    │      │ Extraction   │
                      └──────────────┘      └──────────────┘
        │                     │                     │
        │                     │                     │
        └──────────┬──────────┴─────────────────────┘
                   │
                   ▼
         ┌──────────────────────┐
         │    Mission Brief     │
         │   (Mission Data)     │
         │                      │
         │ • District           │
         │ • Opposition         │
         │ • Archetype          │
         │ • Weapon             │
         │ • Complication       │
         │ • Extraction         │
         └──────────────────────┘
```

---

## Game Loop

**Complete mission-to-gameplay flow:**

1. **Mission Generated** → Random FMissionBrief created from lore data
2. **Enemies Spawn** → 3 enemies from Opposition faction appear with weapons
3. **Hazards Spawn** → 2-3 environmental hazards place randomly
4. **HUD Displays** → Mission briefing and objectives shown to player
5. **Combat** → Player and enemies shoot, take damage, die
6. **Resolution** → Player dies (game over) or reaches extraction (victory)

---

## Configuration & Customization

All gameplay parameters are configurable in Blueprint Details:

**Enemy Difficulty:**
- Detection Range (how far they see)
- Fire Rate (faster shooting)
- Max Health (tougher enemies)
- Retreat Threshold (more/less aggressive)

**Hazard Intensity:**
- Damage Per Second (stronger hazards)
- Effect Radius (bigger danger zones)
- Hazard Type (different damage types)

**Mission Configuration:**
- Enemy Count (more/fewer enemies per mission)
- Hazard Count (more/fewer hazards per mission)

---

## Next Steps

### For Players
1. Follow [BLUEPRINT_SETUP_GUIDE.md](BLUEPRINT_SETUP_GUIDE.md) to set up blueprints
2. Create test level with walkable geometry
3. Compile C++ and build NavMesh
4. Press Play and enjoy!

### For Developers
1. See [COMPLETE_GAMEPLAY_IMPLEMENTATION.md](COMPLETE_GAMEPLAY_IMPLEMENTATION.md) for technical details
2. See [GAME_DEVELOPMENT.md](GAME_DEVELOPMENT.md) for the development roadmap
3. Review [ENEMY_AI_INTEGRATION.md](ENEMY_AI_INTEGRATION.md) for AI architecture
4. Explore code comments for implementation details

### Future Enhancements
- Advanced AI tactics (flanking, coordinated attacks)
- More enemy types (heavy, fast, ranged)
- Dynamic hazards (moving, pulsing, interacting)
- Player abilities from archetype system
- Mission progression and difficulty scaling
- Campaign structure

---

## Engine Requirements

- **Engine:** Unreal Engine 5.3+ (tested on 5.7)
- **C++ Standard:** C++17
- **Plugins:** None required (uses core Unreal systems)
- **Platform:** Windows 64-bit (should work on others with minimal changes)

---

## Project Status

✅ **Complete:** Mission generation, enemy AI, hazard system, HUD/UI, combat  
⏳ **Optional:** Advanced AI tactics, multiple enemy types, campaign progression  

**Ready to play!** Compile, create blueprints, and launch missions.

---

## Documentation Files

- **[BLUEPRINT_SETUP_GUIDE.md](BLUEPRINT_SETUP_GUIDE.md)** - Complete blueprint creation instructions
- **[COMPLETE_GAMEPLAY_IMPLEMENTATION.md](COMPLETE_GAMEPLAY_IMPLEMENTATION.md)** - Technical reference for all systems
- **[IMPLEMENTATION_COMPLETE.md](IMPLEMENTATION_COMPLETE.md)** - Feature summary and completion status
- **[GAME_DEVELOPMENT.md](GAME_DEVELOPMENT.md)** - Development roadmap (updated with all features)
- **[ENEMY_AI_INTEGRATION.md](ENEMY_AI_INTEGRATION.md)** - Detailed AI system architecture
- **[CLAUDE.md](CLAUDE.md)** - Copilot instructions and project conventions

---

## Git Commits

See commit history for detailed implementation progress:
```
21997ee - Final implementation summary and documentation
2306c35 - Complete gameplay loop (AI, combat, hazards, HUD)
116b0bc - Enemy AI integration (ANeonEnemy, ANeonEnemyController)
```

---

**Neon Ascendant is ready for deployment!** 🚀

   outcomes useful for design documentation or automated tests.

Developers working in C++ can include `MissionGenerator.h` and use the same API:

```cpp
#include "MissionGenerator.h"

void UMyMissionComponent::CreateMissions()
{
    UMissionGenerator* Generator = UMissionGeneratorSingleton::GetGenerator();
    Generator->SeedGenerator(1337);

    TArray<FMissionBrief> Briefs;
    Generator->GenerateMissionBriefs(3, Briefs);
    // TODO: Use the generated briefs to drive encounters.
}
```

## Working with the Repository on Windows

If you are collaborating from Windows and plan to use the GitHub CLI (`gh`) or
other tooling that shells out to Git, make sure that [Git for Windows is
installed](https://gitforwindows.org/) and available on your `PATH` before
running commands such as `gh pr merge`. The GitHub CLI depends on the `git`
executable to fetch branches and create merge commits, and will emit the error
`unable to find git executable in PATH` if Git is missing. Installing Git for
Windows (and restarting your terminal so the environment variables refresh)
resolves the issue.

## License

Released under the MIT License. See `LICENSE` for details.
