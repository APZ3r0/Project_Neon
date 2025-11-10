# Neon Ascendant: Unreal Engine Migration Plan

## Executive Summary

This document outlines the comprehensive migration strategy for moving **Neon Ascendant** from a Python prototype to a full Unreal Engine 5 implementation. The plan preserves all core game design concepts while leveraging Unreal's robust systems for 3D gameplay, networking, and procedural generation.

---

## Table of Contents

1. [Project Architecture](#project-architecture)
2. [Data Model Migration](#data-model-migration)
3. [Core Systems Implementation](#core-systems-implementation)
4. [Gameplay Systems](#gameplay-systems)
5. [Procedural Generation](#procedural-generation)
6. [Networking & Multiplayer](#networking--multiplayer)
7. [UI/UX Systems](#uiux-systems)
8. [Migration Phases](#migration-phases)
9. [Technology Stack](#technology-stack)
10. [Risk Assessment & Mitigation](#risk-assessment--mitigation)

---

## Project Architecture

### Unreal Project Structure

```
NeonAscendant/
├── Content/
│   ├── Data/
│   │   ├── Archetypes/          # Data assets for archetypes
│   │   ├── Abilities/            # Ability definitions
│   │   ├── Weapons/              # Weapon data assets
│   │   ├── Implants/             # Implant definitions
│   │   ├── Districts/            # District configurations
│   │   ├── Factions/             # Faction data
│   │   └── Codex/                # Master data tables
│   ├── Blueprints/
│   │   ├── Characters/
│   │   ├── Weapons/
│   │   ├── Abilities/
│   │   ├── UI/
│   │   └── GameModes/
│   ├── Maps/
│   │   ├── Districts/
│   │   ├── Hideout/
│   │   └── Procedural/
│   ├── Materials/
│   ├── Meshes/
│   └── Audio/
├── Source/
│   └── NeonAscendant/
│       ├── Core/                 # Core game systems
│       ├── Characters/           # Character classes
│       ├── Weapons/              # Weapon system
│       ├── Abilities/            # Ability system
│       ├── Loot/                 # Loot generation
│       ├── Procedural/           # Procedural generation
│       ├── Networking/           # Multiplayer systems
│       └── UI/                   # UI widgets
└── Plugins/
    └── NeonAscendantCore/        # Shared plugin code
```

### Core Module Organization

**Primary Modules:**
- `NeonAscendantCore`: Foundation systems (data models, utilities)
- `NeonAscendantGameplay`: Combat, abilities, character systems
- `NeonAscendantLoot`: Loot generation, rarity, affixes
- `NeonAscendantProcedural`: District generation, mission spawning
- `NeonAscendantNetworking`: Multiplayer, replication, matchmaking

---

## Data Model Migration

### Python → Unreal Mapping

| Python Component | Unreal Equivalent | Implementation |
|-----------------|-------------------|----------------|
| `@dataclass` classes | `USTRUCT()` or Data Assets | C++ structs + Blueprint data assets |
| `build_codex()` | Data Tables / Data Assets | `UDataTable` with CSV import |
| Constants | `UENUM()` / `UCLASS()` | Enums and static classes |
| Random generation | `FRandomStream` | Unreal's RNG system |

### Key Data Structures

#### 1. Archetype System
```cpp
// C++ Base
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
class UArchetypeDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString ArchetypeName;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Role;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FArchetypeProfile BaseStats;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftObjectPtr<UAbilityDataAsset>> Abilities;
};
```

#### 2. Weapon System
```cpp
UENUM(BlueprintType)
enum class EWeaponRarity : uint8
{
    Common,
    Uncommon,
    Rare,
    Epic,
    Ascendant
};

USTRUCT(BlueprintType)
struct FWeaponAffix
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere)
    FString AffixName;
    
    UPROPERTY(EditAnywhere)
    float ModifierValue;
    
    UPROPERTY(EditAnywhere)
    EStatType AffectedStat;
};
```

#### 3. Mission Brief System
```cpp
USTRUCT(BlueprintType)
struct FMissionBrief
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    TSoftObjectPtr<UDistrictDataAsset> District;
    
    UPROPERTY(BlueprintReadOnly)
    TSoftObjectPtr<UFactionDataAsset> Opposition;
    
    UPROPERTY(BlueprintReadOnly)
    TSoftObjectPtr<UArchetypeDataAsset> Archetype;
    
    UPROPERTY(BlueprintReadOnly)
    TSoftObjectPtr<UWeaponDataAsset> PrimaryWeapon;
    
    UPROPERTY(BlueprintReadOnly)
    TSoftObjectPtr<UImplantDataAsset> BackupImplant;
    
    UPROPERTY(BlueprintReadOnly)
    FString Complication;
    
    UPROPERTY(BlueprintReadOnly)
    FString ExtractionCondition;
};
```

### Data Import Strategy

1. **CSV to Data Tables**: Convert Python data definitions to CSV files
2. **Data Asset Creation**: Use Unreal's import system to generate data assets
3. **Codex System**: Create a `UCodexSubsystem` that loads all data tables on startup
4. **Runtime Access**: Provide Blueprint and C++ APIs for querying game data

---

## Core Systems Implementation

### 1. Game Mode & Game State

**Primary Game Modes:**
- `ANeonAscendantGameMode`: Base game mode
- `AExtractionGameMode`: Mission/raid game mode
- `AHideoutGameMode`: Social hub game mode

**Game State:**
- `ANeonAscendantGameState`: Tracks mission state, player progress
- `AExtractionGameState`: Manages extraction timers, objectives

### 2. Player Controller & Character

**Player Controller:**
```cpp
class ANeonAscendantPlayerController : public APlayerController
{
    // Camera switching (ARPG ↔ Third-person)
    // Input handling
    // UI management
    // Ability targeting
};
```

**Character Base:**
```cpp
class ANeonAscendantCharacter : public ACharacter
{
    // Archetype system integration
    // Ability system
    // Implant modifiers
    // Stat system (Stealth, Assault, Tech, Resilience)
    // Loot inventory
};
```

### 3. Subsystem Architecture

**Core Subsystems:**
- `UCodexSubsystem`: Central data access
- `ULootGenerationSubsystem`: Procedural loot creation
- `UMissionGenerationSubsystem`: Mission brief generation
- `UProceduralGenerationSubsystem`: District/level generation
- `UPlayerProgressionSubsystem`: Character progression, unlocks

---

## Gameplay Systems

### 1. Combat System

**Weapon System:**
- Base weapon class with modular components
- Damage calculation with rarity/affix modifiers
- Weapon families (SMG, Railgun, Experimental)
- Projectile/beam/hitscan variants

**Ability System (Gameplay Ability System - GAS):**
- Implement using Unreal's GAS plugin
- Abilities: EMP Burst, Neural Hack, Overdrive, Nanite Swarm
- Cooldowns, costs, targeting
- Synergy bonuses with archetype stats

**Stat System:**
- Four core stats: Stealth, Assault, Tech, Resilience
- Derived stats (damage, health, hack success)
- Implant modifiers
- Temporary buffs/debuffs

### 2. Archetype System

**Implementation:**
- Each archetype as a data asset
- Character class selection at mission start
- Ability loadouts per archetype
- Visual customization (meshes, materials, VFX)

**Archetypes:**
1. **Specter**: Stealth-focused, cloaking, hacking
2. **Juggernaut**: Tank, heavy weapons, melee
3. **Tracer**: Mobility, dual-wield, speed boosts
4. **Synthmage**: Tech control, drones, nanites

### 3. Loot System

**Loot Generation:**
- Rarity tiers (Common → Ascendant)
- Affix generation with weighted tables
- Weapon stat randomization
- Implant stat rolls

**Loot Drops:**
- Enemy loot tables
- Container loot
- Mission completion rewards
- Extraction bonuses

**Inventory:**
- Stash system (persistent)
- Mission inventory (lost on death)
- Extraction validation

### 4. Extraction System

**Core Mechanics:**
- Extraction zones/points
- Timer-based extraction
- Condition-based extraction (e.g., "maintain 50% armor")
- Failure = lose mission inventory

**Implementation:**
- `AExtractionZone` actor
- `UExtractionSubsystem` manages zones
- UI indicators for extraction points
- Countdown timers

---

## Procedural Generation

### District Generation

**Approach:**
- Modular level pieces (splines, prefabs)
- Procedural mesh generation for base geometry
- Placed props, enemies, loot containers
- Hazard zones (toxic areas, turret nests)

**Tools:**
- Unreal's Procedural Mesh Component
- PCG (Procedural Content Generation) plugin (UE 5.2+)
- Custom Blueprint nodes for district assembly

**District Types:**
1. **Neon Abyss**: Tight corridors, verticality, gang hideouts
2. **Spire District**: Corporate towers, open spaces, security
3. **Ghost Grid**: Digital overlay, reality distortion, AI anomalies

### Mission Generation

**Mission Brief System:**
- Port Python `MissionGenerator` to C++
- Runtime mission generation
- Seed-based reproducibility
- Mission parameters (difficulty, district, archetype)

**Mission Phases:**
1. **Infiltration**: Stealth challenges
2. **Combat Clash**: Enemy encounters
3. **Ghost Capture**: Objective completion
4. **Extraction**: Escape sequence

---

## Networking & Multiplayer

### Architecture

**Framework:**
- Unreal's built-in networking (replication)
- Dedicated server model for extraction missions
- Peer-to-peer or listen server for hideout

**Replication Strategy:**
- Character state (health, position, stats)
- Weapon/ability usage
- Loot drops (server-authoritative)
- Mission state synchronization

### Co-op Features

**1-4 Player Co-op:**
- Instanced loot (no theft)
- Scaling difficulty
- Shared objectives
- Revive mechanics

**Matchmaking:**
- Quick match (random district)
- Custom match (select district/archetype)
- Private lobbies

### Dark Zone (PvP)

**Optional PvP Mode:**
- Separate game mode
- High-risk, high-reward
- Player-versus-player extraction
- Bounty system

---

## UI/UX Systems

### HUD Design

**Holographic UI Theme:**
- Cyberpunk aesthetic (magenta, cyan, amber)
- Glitch effects, scan lines
- Projected from cyberdeck (diegetic UI)

**Core HUD Elements:**
- Health/Armor bars
- Ability cooldowns
- Weapon ammo/status
- Mission objectives
- Extraction timer/zone indicator
- Minimap (ghost grid overlay)

### Menu Systems

**Hideout Menus:**
- Character selection/customization
- Loadout management
- Stash/inventory
- Crafting/upgrading
- Mission selection
- Social (friends, party)

**In-Mission UI:**
- Contextual prompts
- Loot notifications
- Ability targeting reticle
- Damage numbers (optional)

### Implementation

**Framework:**
- Unreal's UMG (Unreal Motion Graphics)
- Custom widget components
- Material-based effects for holographic look

---

## Migration Phases

### Phase 1: Foundation (Weeks 1-4)
**Goal:** Establish core architecture and data systems

**Tasks:**
- [ ] Set up Unreal project structure
- [ ] Create C++ modules
- [ ] Port data models (structs, enums)
- [ ] Implement `UCodexSubsystem`
- [ ] Create data asset system
- [ ] Import Python data to CSV/Data Tables
- [ ] Basic character controller
- [ ] Simple third-person camera

**Deliverables:**
- Working project with data systems
- Character can move in test level
- Data accessible via Blueprint/C++

### Phase 2: Core Gameplay (Weeks 5-10)
**Goal:** Implement combat and character systems

**Tasks:**
- [ ] Weapon system (base classes, firing)
- [ ] Ability system (GAS integration)
- [ ] Stat system (four core stats)
- [ ] Archetype system (selection, modifiers)
- [ ] Basic AI enemies
- [ ] Damage/health system
- [ ] Ability implementations (EMP, Neural Hack, etc.)

**Deliverables:**
- Playable combat loop
- All four archetypes functional
- Basic enemy encounters

### Phase 3: Loot & Progression (Weeks 11-14)
**Goal:** Implement loot generation and inventory

**Tasks:**
- [ ] Loot generation subsystem
- [ ] Rarity/affix system
- [ ] Weapon stat randomization
- [ ] Inventory system (stash + mission)
- [ ] Loot drops from enemies
- [ ] Extraction validation
- [ ] Basic progression tracking

**Deliverables:**
- Full loot system
- Players can collect and extract loot
- Rarity/affixes working

### Phase 4: Procedural Generation (Weeks 15-20)
**Goal:** District and mission generation

**Tasks:**
- [ ] District generation system
- [ ] Modular level pieces
- [ ] Mission brief generation (port Python code)
- [ ] Objective system
- [ ] Extraction zones
- [ ] At least one complete district (Neon Abyss)

**Deliverables:**
- Procedurally generated missions
- Full mission loop (infiltration → combat → objective → extraction)

### Phase 5: Multiplayer (Weeks 21-26)
**Goal:** Co-op functionality

**Tasks:**
- [ ] Networking setup
- [ ] Character replication
- [ ] Loot replication (instanced)
- [ ] Matchmaking system
- [ ] Party system
- [ ] Hideout as social hub
- [ ] Testing and optimization

**Deliverables:**
- 1-4 player co-op working
- Matchmaking functional
- Stable networking

### Phase 6: Polish & Content (Weeks 27-32)
**Goal:** Additional districts, UI polish, balancing

**Tasks:**
- [ ] Remaining districts (Spire, Ghost Grid)
- [ ] UI/UX polish (holographic theme)
- [ ] Audio integration
- [ ] VFX for abilities
- [ ] Balancing pass
- [ ] Performance optimization
- [ ] Bug fixes

**Deliverables:**
- All three districts playable
- Polished UI
- Balanced gameplay

### Phase 7: Endgame & Launch Prep (Weeks 33-40)
**Goal:** Endgame systems and release readiness

**Tasks:**
- [ ] Ghost Runs (procedural rifts)
- [ ] Faction War system
- [ ] Ascendant Trials
- [ ] Implant Ascension Trees
- [ ] Narrative integration
- [ ] Localization
- [ ] Platform-specific optimizations
- [ ] QA and bug fixing

**Deliverables:**
- Complete game with endgame
- Ready for early access or launch

---

## Technology Stack

### Engine & Tools
- **Unreal Engine 5.3+** (latest stable)
- **Visual Studio 2022** (C++ development)
- **Perforce/Git LFS** (version control for large assets)

### Plugins & Extensions
- **Gameplay Ability System (GAS)** - Ability framework
- **PCG Plugin** - Procedural content generation (UE 5.2+)
- **Online Subsystem** - Matchmaking/networking
- **Enhanced Input** - Modern input system

### External Tools
- **Blender/Maya** - 3D asset creation
- **Substance Designer/Painter** - Material creation
- **FMOD/Wwise** - Audio middleware
- **Perforce Helix/Git** - Version control

### Platform Targets
- **Primary:** PC (Steam, Epic Games Store)
- **Secondary:** PlayStation 5, Xbox Series X/S
- **Future:** Cloud gaming platforms

---

## Risk Assessment & Mitigation

### Technical Risks

| Risk | Impact | Probability | Mitigation |
|------|--------|-------------|------------|
| Procedural generation performance | High | Medium | Use PCG plugin, optimize early, LOD systems |
| Networking complexity | High | High | Start with simple replication, iterate |
| Loot system balance | Medium | High | Extensive playtesting, data-driven tuning |
| Scope creep | High | Medium | Strict phase gates, MVP-first approach |

### Development Risks

| Risk | Impact | Probability | Mitigation |
|------|--------|-------------|------------|
| Team size/experience | High | Medium | Hire Unreal specialists, training |
| Timeline delays | High | High | Buffer time in phases, prioritize core features |
| Asset pipeline bottlenecks | Medium | Medium | Establish pipeline early, use marketplace assets initially |

### Mitigation Strategies

1. **Prototype Early**: Build vertical slice in Phase 2 to validate core loop
2. **Iterative Development**: Test each phase before moving to next
3. **Community Feedback**: Early access for balancing and feedback
4. **Modular Architecture**: Systems can be developed in parallel
5. **Asset Reuse**: Leverage Unreal Marketplace for placeholder assets

---

## Success Metrics

### Technical Metrics
- 60 FPS on target hardware (1080p, medium settings)
- <100ms network latency for co-op
- <5 second mission generation time
- Stable 4-player co-op performance

### Gameplay Metrics
- Mission completion rate: 60-70%
- Average mission duration: 15-25 minutes
- Loot drop satisfaction (player surveys)
- Archetype balance (usage distribution)

### Development Metrics
- On-time phase completion: 80%+
- Bug density: <5 critical bugs per phase
- Code coverage: 70%+ for core systems

---

## Next Steps

### Immediate Actions (Week 1)
1. Set up Unreal Engine 5 project
2. Create project structure and modules
3. Port data models to C++ structs
4. Set up version control (Git LFS or Perforce)
5. Create initial data assets from Python codex

### Team Requirements
- **Lead Programmer** (Unreal/C++ expertise)
- **Gameplay Programmer** (combat, abilities)
- **Network Programmer** (multiplayer systems)
- **Technical Artist** (procedural generation, VFX)
- **UI/UX Designer** (holographic UI design)
- **3D Artists** (characters, weapons, environments)
- **Audio Designer** (synthwave soundtrack, SFX)

### Resource Allocation
- **Development:** 70% (core systems, gameplay)
- **Art/Assets:** 20% (visuals, audio)
- **QA/Testing:** 10% (playtesting, bug fixing)

---

## Conclusion

This migration plan provides a structured approach to bringing **Neon Ascendant** to Unreal Engine 5. By following the phased approach, maintaining the core design vision, and leveraging Unreal's robust systems, the project can successfully transition from prototype to full game.

**Key Success Factors:**
1. Maintain design vision from Python prototype
2. Leverage Unreal's strengths (GAS, networking, procedural tools)
3. Iterative development with early playtesting
4. Modular architecture for parallel development
5. Clear phase gates and deliverables

The estimated timeline of 40 weeks assumes a full team. Adjust based on team size and resources. Prioritize core gameplay loop (Phases 1-4) before expanding to multiplayer and endgame content.

---

**Document Version:** 1.0  
**Last Updated:** 2024  
**Next Review:** After Phase 1 completion

