# NeonGameMode & Enemy Spawning Integration Guide

## Overview

This document explains how **NeonGameMode** integrates the mission generator with gameplay, specifically focusing on **Step 2: Add Enemy AI** from [GAME_DEVELOPMENT.md](GAME_DEVELOPMENT.md).

---

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    ANeonGameMode                             │
├─────────────────────────────────────────────────────────────┤
│ • Manages game flow and mission generation                   │
│ • Coordinates with UMissionGenerator for random missions     │
│ • Spawns enemies based on mission Opposition faction         │
│ • Configures level hazards and objectives                    │
└─────────────────────────────────────────────────────────────┘
          │                           │
          ├──────────────────────────┤
          ▼                          ▼
┌──────────────────────┐    ┌───────────────────────┐
│   UMissionGenerator  │    │  Enemy Spawning       │
├──────────────────────┤    ├───────────────────────┤
│ • Generates random   │    │ • ANeonEnemy (AI)     │
│   mission briefs     │    │ • ANeonEnemyController│
│ • Uses FRandomStream │    │ • NavMesh pathfinding │
│ • Returns FMissionBr │    │ • Detection/chasing   │
└──────────────────────┘    └───────────────────────┘
          │                          │
          └──────────────────────────┘
                    │
                    ▼
         ┌──────────────────────┐
         │   Mission Brief      │
         ├──────────────────────┤
         │ • Opposition faction │
         │ • District hazards   │
         │ • Complications      │
         │ • Extraction point   │
         └──────────────────────┘
```

---

## Files & Classes

### Core Classes

| File | Class | Purpose |
|------|-------|---------|
| `NeonGameMode.h/.cpp` | `ANeonGameMode` | Game mode - orchestrates mission generation and enemy spawning |
| `NeonEnemy.h/.cpp` | `ANeonEnemy` | Enemy character with health, detection, and simple AI |
| `NeonEnemyController.h/.cpp` | `ANeonEnemyController` | AI controller - pathfinding and player targeting |

### Key Functions

#### `ANeonGameMode::StartNewMission()`
```cpp
void ANeonGameMode::StartNewMission()
{
    // 1. Get mission generator singleton
    // 2. Generate random FMissionBrief
    // 3. Log mission details
    // 4. Call SpawnEnemiesForMission(NewMission)
}
```

**Called when:**
- Game starts (BeginPlay)
- Player requests new mission
- Previous mission complete

**Does:**
- Generates deterministic mission from `FRandomStream`
- Spawns Opposition faction enemies
- Configures level based on district

---

#### `ANeonGameMode::SpawnEnemiesForMission()`
```cpp
void ANeonGameMode::SpawnEnemiesForMission(
    const FMissionBrief& Mission, 
    int32 EnemyCount = 3)
{
    // 1. Validate EnemyClass is set in Blueprint
    // 2. Find spawn points in level (or use random positions)
    // 3. Spawn EnemyCount actors at those locations
    // 4. Log spawn information
}
```

**Takes:**
- `FMissionBrief` - Contains Opposition faction, complication, district data
- `EnemyCount` - Number of enemies to spawn (default 3)

**Does:**
- Creates `ANeonEnemy` instances using `EnemyClass` Blueprint
- Positions them strategically in level
- Logs spawn locations for debugging

**Requirements:**
- `EnemyClass` must be assigned in Blueprint editor
- `ANeonEnemy` class must exist (now it does!)
- NavMesh must be present for pathfinding

---

## Step 2 Implementation Checklist

### ✅ Phase 1: C++ Classes (COMPLETED)

- [x] `ANeonEnemy` - Enemy character class
  - [x] Health system (MaxHealth, CurrentHealth)
  - [x] TakeDamage() function
  - [x] Die() function with ragdoll
  - [x] Detection range configuration
  - [x] Simple tick-based AI (chase player if detected)

- [x] `ANeonEnemyController` - AI controller
  - [x] Line-of-sight detection
  - [x] Pathfinding movement toward player
  - [x] Integration with NavMesh system

- [x] `ANeonGameMode` updates
  - [x] `SpawnEnemiesForMission()` function
  - [x] Integration with mission generator
  - [x] Enemy class property for Blueprint assignment

### ⏳ Phase 2: Blueprints (YOU DO THIS)

1. **Create BP_NeonEnemy Blueprint**
   - File → New → Blueprint Class
   - Choose Parent Class: `ANeonEnemy`
   - Name: `BP_NeonEnemy`
   - Assign skeletal mesh (or use default capsule)
   - Set collision settings
   - Set AI Controller Class: `ANeonEnemyController`

2. **Create BP_NeonGameMode Blueprint**
   - File → New → Blueprint Class
   - Choose Parent Class: `ANeonGameMode`
   - Name: `BP_NeonGameMode`
   - In Details → Mission section:
     - Set **Enemy Class** property to `BP_NeonEnemy`

3. **Set World Settings**
   - Open TestLevel
   - World Settings → Game Mode Override
   - Set to `BP_NeonGameMode`

### ⏳ Phase 3: Navigation Setup (YOU DO THIS)

1. **Add NavMesh Bounds Volume**
   - Place → Volume → Nav Mesh Bounds Volume
   - Scale to cover all walkable areas
   - Click **Build** → **Build Paths**
   - Green overlay shows nav mesh

2. **Test Navigation**
   - Play level
   - Enemies should move toward you
   - Use Shift+P to visualize AI navigation

---

## Mission → Gameplay Flow

```
1. Call StartNewMission()
   ↓
2. Generate FMissionBrief
   ├─ District: Which location
   ├─ Opposition: Enemy faction type
   ├─ Complication: Environmental hazard
   └─ Extraction: Victory condition
   ↓
3. Log mission details
   ↓
4. Call SpawnEnemiesForMission(Mission)
   ├─ Read Opposition faction from Mission
   ├─ Determine enemy type/difficulty
   ├─ Find spawn points in level
   └─ Spawn ANeonEnemy actors
   ↓
5. Game loop
   ├─ Player moves/fights
   ├─ Enemies chase (ANeonEnemyController)
   ├─ Enemies take damage (ANeonEnemy::TakeDamage)
   └─ Enemies die (ANeonEnemy::Die)
   ↓
6. Victory
   └─ Check if extraction condition met
```

---

## Important: Mission Data → Difficulty Mapping

Currently, `SpawnEnemiesForMission()` spawns a fixed number of generic enemies. To make it **authentic to your lore**, extend it:

```cpp
void ANeonGameMode::SpawnEnemiesForMission(const FMissionBrief& Mission, int32 EnemyCount)
{
    // TODO: Use Mission.Opposition.Name to determine:
    // - Enemy health (different factions have different durability)
    // - Enemy weapons (match Opposition weapon preferences)
    // - Spawn count (Synth Collective might send more, Ghost Squad fewer but tougher)
    // - Tactics (Armor Corp = defensive, Ghosts = evasive, etc.)
    
    // For now, all enemies are identical
}
```

See [GAME_DEVELOPMENT.md](GAME_DEVELOPMENT.md) **Step 4: Connect Mission Generator to Gameplay** for guidance on this.

---

## Debugging

### Play Level & Verify Enemies Spawn

```cpp
// In BP_NeonGameMode Details:
// Mission section → Enemy Class: BP_NeonEnemy
// 
// This ensures SpawnEnemiesForMission() has a valid class to instantiate
```

### Check Logs
```
LogTemp: New Mission Generated:
  District: Neon Sprawl
  Opposition: Synth Collective
  Archetype: Specter
  Weapon: Pulsecaster SMG
  Complication: Guard Network Active
  Extraction: Secure Data Terminal
LogTemp: Spawning 3 enemies for mission vs Synth Collective
LogTemp: Spawned enemy 1 at location (123, 456, 100)
LogTemp: Spawned enemy 2 at location (234, 567, 100)
LogTemp: Spawned enemy 3 at location (345, 678, 100)
```

### Common Issues

**Problem:** "Enemies don't spawn"
- Solution: Ensure `EnemyClass` is set in BP_NeonGameMode Details
- Check: Blueprint Property → Mission → Enemy Class = BP_NeonEnemy

**Problem:** "Enemies spawn but don't move"
- Solution: Verify NavMesh Bounds Volume is placed and built
- Check: Shift+P toggles AI debug visualization

**Problem:** "Enemies clip through walls"
- Solution: Check capsule collision settings on BP_NeonEnemy
- Check: NavMesh generation includes all intended pathways

---

## Next Steps

After completing Phase 2 & 3:

1. **Playtest**: Verify enemies spawn, chase, and die
2. **Tune difficulty**: Adjust health/spawn counts per mission
3. **Add variety**: Create multiple enemy archetypes (fast/tanky/ranged)
4. **Implement district hazards**: Lava, electrical fields, etc. (Step 3)
5. **Add HUD**: Display mission objective and enemy count (Step 5)

---

## Related Documentation

- [GAME_DEVELOPMENT.md](GAME_DEVELOPMENT.md) - Full development roadmap
- [CLAUDE.md](CLAUDE.md) - Architecture and component overview
- Mission Generator API - See `MissionGenerator.h`
- Enemy Character - See `NeonEnemy.h`
- Game Mode - See `NeonGameMode.h`
