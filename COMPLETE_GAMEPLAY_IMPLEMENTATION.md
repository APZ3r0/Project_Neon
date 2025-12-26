# Complete Gameplay Loop Implementation Guide

## Overview

All five major features have been implemented to create a complete, playable gameplay loop:

1. **Advanced AI System** - State machine with patrol, investigation, engagement, retreat
2. **Enemy Combat** - Autonomous weapon firing with intelligent aiming
3. **Player Health System** - Damage tracking, death mechanics, ragdoll
4. **District Hazards** - Environmental damage zones matching district themes
5. **HUD/UI** - Mission briefing, health tracking, ammo counter, objective display

---

## 1. Advanced AI System (ANeonEnemyController)

### State Machine

Enemies operate in a 5-state system:

```
┌─────────────────────────────────────────┐
│         PATROL                           │
│ - Move randomly in area                  │
│ - Wait at patrol points                  │
│ - Detects player entering range          │
└─────────────────────────────────────────┘
          │ (Player detected)
          ▼
┌─────────────────────────────────────────┐
│         ENGAGED                          │
│ - Chase player at high speed             │
│ - Update last known position             │
│ - Check health for retreat               │
│ - Continue if health drops below 25%     │
└─────────────────────────────────────────┘
          │ (Damage taken)      │ (Lost sight)
          ▼                     ▼
    [RETREAT]           [INVESTIGATE]
          │                     │
          └─────────┬───────────┘
                    ▼
            (Recovery/Timeout)
                    │
                    ▼
            [PATROL]
```

### Configuration Properties

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Detection")
float DetectionRange = 2000.0f;  // How far enemies can "see"

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Detection")
float LostTargetDistance = 3000.0f;  // Distance before giving up chase

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
float PatrolSpeed = 300.0f;  // Movement speed during patrol

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Retreat")
float RetreatHealthThreshold = 0.25f;  // Retreat when below 25% health
```

### Behavior Details

- **Patrol**: Randomly generated waypoints within 1500-unit radius
- **Investigate**: Moves toward last known damage location, waits 5 seconds
- **Engaged**: Moves toward player at 800 units/sec, fires when in range
- **Retreat**: Moves away at 1000 units/sec, rejoins if health recovers
- **Line-of-Sight**: Traces from enemy head to player head to validate targeting

---

## 2. Enemy Combat System (ANeonEnemy + Weapon Integration)

### Weapon Equipping

```cpp
// In Blueprint (BP_NeonEnemy):
// Set WeaponClass property to any ANeonWeapon subclass
// Weapon automatically spawns and attaches on BeginPlay

EquipWeapon();  // Called in BeginPlay()
// - Spawns weapon actor from WeaponClass
// - Attaches to "WeaponSocket" on skeleton
// - Logs successful equip
```

### Autonomous Firing

```cpp
// In Tick(), when in attack range (< AttackRange):
void ANeonEnemy::FireWeapon()
{
    // 1. Calculate aim point toward player
    FVector AimStart = GetActorLocation() + Head offset;
    FVector AimEnd = TargetPlayer->GetActorLocation() + 50cm;
    
    // 2. Add inaccuracy (20% deviation)
    FVector FinalAim = AimDirection + FMath::VRand() * 0.2f;
    
    // 3. Fire weapon
    EquippedWeapon->Fire(AimStart, FinalAim);
}
```

### Weapon Configuration

```cpp
// In BP_NeonEnemy Details:
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
TSubclassOf<ANeonWeapon> WeaponClass;  // Assign in Blueprint

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
float FireInterval = 0.15f;  // 0.15 seconds between shots
```

---

## 3. Player Health System (ANeonCharacter)

### Health Properties

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
float MaxHealth = 100.0f;

UPROPERTY(BlueprintReadOnly, Category = "Health")
float CurrentHealth = 100.0f;  // Initialized to MaxHealth in constructor
```

### Taking Damage

```cpp
void ANeonCharacter::TakeDamage(float Damage)
{
    CurrentHealth -= Damage;
    
    if (CurrentHealth <= 0.0f)
    {
        Die();  // Ragdoll, disable movement, destroy after 10 seconds
    }
}
```

### Utility Functions

```cpp
UFUNCTION(BlueprintPure, Category = "Health")
float GetHealth() const { return CurrentHealth; }

UFUNCTION(BlueprintPure, Category = "Health")
float GetMaxHealth() const { return MaxHealth; }

UFUNCTION(BlueprintPure, Category = "Health")
float GetHealthPercent() const { return CurrentHealth / MaxHealth; }
```

### Death Mechanics

- Unpossess controller
- Stop firing
- Disable movement
- Enable ragdoll physics
- Destroy actor after 10 seconds

---

## 4. District Hazards (ADistrictHazard)

### Hazard Types

```cpp
UENUM(BlueprintType)
enum class EHazardType : uint8
{
    Thermal = 0,      // Red - Heat damage
    Electrical = 1,   // Yellow - Shock damage
    Toxic = 2,        // Green - Poison damage
    Radiation = 3,    // Cyan - Radiation damage
    Cryogenic = 4     // Blue - Freeze damage
};
```

### Configuration

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
EHazardType HazardType = EHazardType::Thermal;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
float DamagePerSecond = 10.0f;  // Damage rate while in zone

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
float EffectRadius = 500.0f;  // Sphere radius of hazard

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
float DamageTickRate = 1.0f;  // Apply damage every X seconds (prevents spam)
```

### Damage Application

- Uses sphere collision to detect overlaps (player, enemies)
- Applies damage at `DamagePerSecond * DamageTickRate` intervals
- Tracks per-actor last damage time to prevent spam
- Applies to both `ANeonCharacter` and `ANeonEnemy`

### Mission Integration

In `ANeonGameMode::SpawnHazardsForMission()`:
```cpp
// Spawns 2-3 randomized hazards per mission
int32 HazardCount = FMath::RandRange(2, 3);

for each hazard:
    NewHazard->HazardType = Random(0-4);
    NewHazard->DamagePerSecond = Random(5-15);
    NewHazard->EffectRadius = Random(300-600);
    ActiveHazards.Add(NewHazard);
```

---

## 5. HUD/UI System (ANeonHUD)

### Elements Displayed

#### Health Bar
- Position: Top-left (20, 20)
- Size: 300x30 pixels
- Color-coded:
  - Green: > 50% health
  - Yellow: 25-50% health
  - Red: < 25% health
- Shows numerical health (e.g., "Health: 75 / 100")

#### Ammo Counter
- Position: Bottom-left
- Updates from equipped weapon: `"Ammo: 15 / 30"`
- Linked to `ANeonWeapon->GetCurrentAmmo()`

#### Mission Briefing Panel
- Position: Left side (y=70)
- Shows:
  - District name
  - Opposition faction
  - Player archetype
  - Primary weapon equipped
  - Complication (environmental modifier)
  - Extraction condition (win condition)

#### Objective Tracker
- Position: Left side (y=400)
- Displays:
  - Current complication (objective)
  - Extraction method (how to win)

#### Extraction Indicator
- Position: Top-right
- Shows: "Extraction: ACTIVE" in green
- Visual indicator that mission is in progress

### Configuration

All HUD positions are configurable:

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Layout")
FVector2D HealthBarPosition = FVector2D(20.0f, 20.0f);

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Layout")
FVector2D MissionBriefingPosition = FVector2D(20.0f, 70.0f);

// ... etc for all UI elements
```

### Color Customization

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
FColor HealthyColor = FColor::Green;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
FColor DamagedColor = FColor::Yellow;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
FColor CriticalColor = FColor::Red;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
FColor TextColor = FColor::White;
```

---

## Complete Game Flow

```
┌─────────────────────────────────────────────────────────────┐
│ 1. Level Loads                                               │
│    - ANeonGameMode spawns (DefaultPawnClass = ANeonCharacter)│
│    - ANeonHUD spawns (HUDClass = ANeonHUD)                   │
│    - BeginPlay() called on all actors                        │
└─────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────┐
│ 2. Mission Generation                                        │
│    - Blueprint calls GameMode->StartNewMission()            │
│    - UMissionGenerator creates random FMissionBrief         │
│    - Details logged to output                               │
└─────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────┐
│ 3. Enemy Spawning                                            │
│    - SpawnEnemiesForMission(Mission) called                 │
│    - 3 enemies spawned at random locations                  │
│    - Each enemy:                                             │
│      - Gets assigned ANeonEnemyController                    │
│      - Enters PATROL state                                  │
│      - Equips configured WeaponClass                        │
└─────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────┐
│ 4. Hazard Spawning                                           │
│    - SpawnHazardsForMission(Mission) called                 │
│    - 2-3 hazards spawned at random locations                │
│    - Each hazard:                                            │
│      - Random type (Thermal, Electrical, Toxic, etc)        │
│      - Random damage (5-15 dps)                             │
│      - Random radius (300-600 units)                        │
└─────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────┐
│ 5. HUD Display                                               │
│    - HUD->SetMissionBrief(Mission)                          │
│    - HUD->SetPlayerCharacter(PlayerCharacter)               │
│    - Briefing panel shows mission details                   │
└─────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────┐
│ 6. Combat Loop (Tick)                                        │
│                                                               │
│  Player:                                                     │
│  - Move with WASD, look with mouse                          │
│  - Fire with LMB (hits enemies with hitscan)                │
│  - Enemies take damage -> die -> ragdoll                    │
│  - Run through hazards -> take damage -> die if health=0    │
│                                                               │
│  Enemies:                                                    │
│  - Patrol (PATROL state) until player spotted               │
│  - Chase player (ENGAGED state) when in range               │
│  - Fire weapons with inaccuracy at player                   │
│  - Retreat when health < 25% (RETREAT state)                │
│  - Investigate last known location if line-of-sight lost    │
│  - Take damage from player shots and hazards                │
│                                                               │
│  HUD Updates (every frame):                                  │
│  - Health bar color-codes based on health%                  │
│  - Ammo counter updates from weapon                         │
│  - Briefing shows current objective                         │
│  - Extraction indicator shows mission active                │
└─────────────────────────────────────────────────────────────┘
                             │
                      ┌──────┴──────┐
                      ▼             ▼
        ┌──────────────────┐  ┌──────────────────┐
        │ Player Death     │  │ Mission Victory  │
        │ - Health → 0     │  │ - Reach exit     │
        │ - Ragdoll        │  │ - Extract        │
        │ - Destroy (10s)  │  │ - Next mission?  │
        │ - Game Over      │  │ - Briefing reset │
        └──────────────────┘  └──────────────────┘
```

---

## Next Steps: Testing & Refinement

### Blueprint Setup Required

1. **Create BP_NeonEnemy**
   - Parent: `ANeonEnemy`
   - Set `WeaponClass` → `BP_AssaultRifle` (or any weapon)
   - Set `AI Controller Class` → `ANeonEnemyController`

2. **Create BP_DistrictHazard** (optional, can use `ADistrictHazard` directly)
   - Parent: `ADistrictHazard`
   - Customize colors/effects

3. **Set Game Mode in Level**
   - World Settings → Game Mode Override → `ANeonGameMode`

4. **Ensure NavMesh is built**
   - Build → Build Paths
   - Green overlay shows valid pathfinding areas

### Testing Checklist

- [ ] Enemies spawn and patrol
- [ ] Player shot enemies take damage
- [ ] Enemies detect and chase player
- [ ] Enemies fire back at player
- [ ] Enemies retreat when low health
- [ ] Enemies investigate when losing sight
- [ ] Hazards spawn and apply damage
- [ ] Player health bar updates
- [ ] Mission briefing displays correctly
- [ ] Ammo counter tracks weapon ammo
- [ ] Extraction indicator visible

### Performance Optimization

- Enemy tick interval: 0.2 seconds (10 checks/sec)
- Hazard damage tick: Configurable per hazard (prevent CPU spam)
- Line-of-sight checks only in ENGAGED state
- Lazy-initialize components on first use

---

## File Reference

| File | Purpose |
|------|---------|
| [NeonEnemyController.h/cpp](../Source/NeonAscendant/Public/NeonEnemyController.h) | AI state machine & behavior |
| [NeonEnemy.h/cpp](../Source/NeonAscendant/Public/NeonEnemy.h) | Enemy character & weapon integration |
| [DistrictHazard.h/cpp](../Source/NeonAscendant/Public/DistrictHazard.h) | Environmental damage zones |
| [NeonHUD.h/cpp](../Source/NeonAscendant/Public/NeonHUD.h) | Canvas-based UI rendering |
| [NeonGameMode.cpp](../Source/NeonAscendant/Private/NeonGameMode.cpp) | Mission orchestration |
| [NeonCharacter.h/cpp](../Source/NeonAscendant/Public/NeonCharacter.h) | Player health & death |

---

## Architecture Summary

```
┌─────────────────────────────────────────────────┐
│         ANeonGameMode                            │
│  (Orchestrator: generates missions/enemies/HUD)  │
├─────────────────────────────────────────────────┤
│  StartNewMission()                               │
│  ├─ SpawnEnemiesForMission()                    │
│  ├─ SpawnHazardsForMission()                    │
│  └─ HUD->SetMissionBrief()                      │
└─────────────────────────────────────────────────┘
         │                │                │
         ▼                ▼                ▼
    [Enemies]        [Hazards]          [HUD]
         │                │                │
    ┌────┴─────┐      ┌────┴─────┐       │
    ▼          ▼      ▼          ▼       ▼
 ANeonEnemy  Controller  Hazard  Damage  ANeonHUD
    │          │                │        │
    └─→ FireWeapon()            └─→ DrawAll()
       Chase/Patrol                ├─ Health
       TakeDamage()                ├─ Ammo
                                   ├─ Mission
                                   └─ Objectives
```

This architecture ensures clean separation of concerns while integrating all systems for complete gameplay.
