# Neon Ascendant - Unity Edition

This is the Unity conversion of the Neon Ascendant tactical sci-fi game, originally built in Unreal Engine 5.7.

## Project Overview

**Neon Ascendant** is a tactical sci-fi game with a deterministic mission generator. The codebase includes:
- C# runtime scripts with mission generation, character, and weapon systems
- Unity-compatible APIs for all gameplay features
- Core gameplay foundation: player character, weapons, and game management

**Key Philosophy**: The mission generator is central to the game design. All lore data (districts, factions, archetypes, weapons, implants) is hard-coded in C# for deterministic, seeded random generation using `System.Random`.

## Unity Version

- **Recommended**: Unity 2022.3 LTS or later
- **Required Packages**:
  - TextMeshPro (for UI)
  - NavMesh Components (for enemy AI)
  - Input System (optional, for enhanced input)

## Project Structure

```
Assets/
├── Scripts/
│   ├── Mission/          # Mission generation system
│   │   ├── MissionTypes.cs         # Data structures
│   │   ├── MissionData.cs          # Hard-coded lore data
│   │   └── MissionGenerator.cs     # Deterministic generator
│   ├── Gameplay/         # Player and weapon systems
│   │   ├── NeonCharacter.cs        # Player controller
│   │   ├── NeonWeapon.cs           # Hitscan weapon system
│   │   └── DistrictHazard.cs       # Environmental hazards
│   ├── Enemy/            # Enemy AI
│   │   └── NeonEnemy.cs            # Basic enemy with NavMesh
│   ├── Core/             # Game management
│   │   └── NeonGameManager.cs      # Mission & spawn management
│   └── UI/               # User interface
│       └── NeonHUD.cs              # Health, ammo, mission display
├── Scenes/               # Unity scenes
└── Prefabs/              # Reusable game objects
```

## Key Systems

### Mission Generation System

The mission generator uses a singleton pattern with deterministic random generation:

```csharp
// Get the generator instance
var generator = MissionGenerator.Instance;

// Seed for deterministic generation (optional)
generator.SeedGenerator(1337);

// Generate single mission
MissionBrief mission = generator.GenerateMissionBrief();

// Generate multiple missions
List<MissionBrief> briefs = generator.GenerateMissionBriefs(5);
```

**Mission Brief Structure**:
- District (location with hazards)
- Opposition faction (tactics and description)
- Archetype (character class with abilities)
- Primary weapon
- Implant (augmentation with effects)
- Complication (difficulty modifier)
- Extraction condition (win condition)

All data is randomly selected from hard-coded arrays in `MissionData.cs` using `System.Random`.

### Gameplay Classes

**NeonCharacter**:
- First-person and third-person camera support
- Movement: WASD, jump, crouch, sprint
- CharacterController-based physics
- Weapon system integration
- Health and damage system
- Default walk speed: 6.0 m/s, sprint multiplier: 1.5x, crouch multiplier: 0.5x

**NeonWeapon**:
- Hitscan shooting using raycasts
- Fire modes: Automatic or semi-automatic
- Ammo system with reload mechanics
- Damage application via IDamageable interface
- Debug visualization in editor

**NeonGameManager**:
- Singleton game manager
- Mission generation on level start
- Enemy and hazard spawning
- Deterministic spawn positioning

**NeonEnemy**:
- NavMesh-based AI
- Player detection and chase behavior
- Health and damage system
- Wandering when player not detected

**DistrictHazard**:
- Environmental damage zones
- Multiple hazard types (Thermal, Electrical, Toxic, Radiation, Cryogenic)
- Configurable damage and radius
- Particle system support

### Input Bindings

Default Unity Input Manager settings:
- W/A/S/D - Movement
- Mouse - Look
- Space - Jump
- Left Shift - Sprint
- Left Ctrl / C - Crouch
- Left Mouse Button - Fire
- R - Reload

## Getting Started

### Setting Up a Scene

1. Create a new scene
2. Add `NeonGameManager` prefab/script to scene
3. Create player GameObject with:
   - `NeonCharacter` component
   - `CharacterController` component
   - First-person and third-person cameras as children
   - Tag as "Player"
4. Create weapon prefab with:
   - `NeonWeapon` component
   - Visual mesh
   - Muzzle transform for bullet origin
5. Create UI Canvas with:
   - `NeonHUD` component
   - Health bar, ammo text, mission briefing panels
6. Assign references in Inspector

### Creating Prefabs

**Weapon Prefab**:
1. Create new GameObject
2. Add `NeonWeapon` component
3. Configure: Damage, FireRate, MaxAmmo, Range, ReloadTime, IsAutomatic
4. Add weapon mesh and muzzle transform
5. Save as prefab

**Enemy Prefab**:
1. Create new GameObject with mesh
2. Add `NeonEnemy` component
3. Add `NavMeshAgent` component
4. Add capsule collider
5. Configure health, detection range, attack range
6. Save as prefab

**Hazard Prefab**:
1. Create new GameObject
2. Add `DistrictHazard` component
3. Add `SphereCollider` (will be set as trigger automatically)
4. Optionally add `ParticleSystem` for visual effects
5. Configure hazard type, damage, radius
6. Save as prefab

### Debugging Mission Generation

Use deterministic seeding to reproduce specific missions:

```csharp
// In your script
var generator = MissionGenerator.Instance;
generator.SeedGenerator(1337);  // Always generates same sequence
MissionBrief mission = generator.GenerateMissionBrief();

// Print mission details to console
Debug.Log($"District: {mission.District.Name}");
Debug.Log($"Opposition: {mission.Opposition.Name}");
```

## Key Conventions

### Component Organization
- All gameplay classes inherit from `MonoBehaviour`
- Use `[SerializeField]` for inspector-editable private fields
- Use public properties for read-only access
- Organize with `[Header("Category")]` attributes

### Random Generation
- **Always use `System.Random` in mission generator** for determinism
- Seed before generating: `generator.SeedGenerator(seed)`
- Unity's `Random` class can be used for non-deterministic effects

### Damage System
- All damageable objects implement `IDamageable` interface
- `TakeDamage(float damage, Vector3 direction, Vector3 hitPoint)` method
- Direction and hit point provided for physics effects

### Performance
- Debug logging wrapped in `#if UNITY_EDITOR || DEVELOPMENT_BUILD`
- Use object pooling for frequently spawned objects (bullets, effects)
- NavMesh baking for enemy pathfinding

## Migration from Unreal Engine

### Key Differences

| Unreal Engine | Unity |
|--------------|--------|
| `AActor` | `MonoBehaviour` |
| `UPROPERTY(EditAnywhere)` | `[SerializeField]` |
| `UFUNCTION(BlueprintCallable)` | `public` method |
| `TArray<T>` | `List<T>` or `T[]` |
| `FVector` | `Vector3` |
| `FRotator` | `Quaternion` or `Vector3` (Euler) |
| `FString` | `string` |
| `UWorld::LineTraceSingle` | `Physics.Raycast` |
| `GetWorld()->SpawnActor` | `Instantiate` |
| `SetTimer` | `Coroutine` or `InvokeRepeating` |

### Architecture Changes

1. **Component System**: Unity uses GameObject + Component instead of Actor inheritance
2. **Input**: Unity Input Manager or new Input System instead of Enhanced Input
3. **Physics**: Unity Physics with raycasts instead of collision channels
4. **AI**: NavMesh and NavMeshAgent instead of AI Controllers
5. **UI**: Unity UI (Canvas) instead of UMG widgets

## Development Workflow

### Adding New Lore Data

**New Archetype**:
1. Open `MissionData.cs`
2. Add new `AscendantArchetype` entry to `GetArchetypes()` list
3. Include name, role, signature, and abilities
4. New archetype auto-included in random selection

**New Weapon/Implant/District/Faction**:
1. Open `MissionData.cs`
2. Add entry to corresponding `Get*()` method
3. Follow existing struct pattern from `MissionTypes.cs`

**New Complication or Extraction Condition**:
1. Open `MissionData.cs`
2. Add string to `GetComplications()` or `GetExtractionConditions()` list

### Creating Custom Abilities

1. Create new class inheriting from `MonoBehaviour`
2. Implement cooldown system using coroutines
3. Add ability reference to character
4. Bind to input in `NeonCharacter` or create separate input handler
5. Reference ability data from mission brief

### Building Game

1. Configure Build Settings (File → Build Settings)
2. Add scenes to build
3. Configure Player Settings
4. Build for target platform (PC, Mac, Linux)

## Common Patterns

### Getting Mission Data in C#

```csharp
using NeonAscendant.Mission;
using NeonAscendant.Core;

public class MyScript : MonoBehaviour
{
    void Start()
    {
        var generator = MissionGenerator.Instance;
        generator.SeedGenerator(System.DateTime.Now.Millisecond);

        List<MissionBrief> briefs = generator.GenerateMissionBriefs(10);

        foreach (var brief in briefs)
        {
            // Use brief.District, brief.Opposition, etc.
        }
    }
}
```

### Spawning Weapons

```csharp
public class NeonCharacter : MonoBehaviour
{
    [SerializeField] private Transform weaponSocket;
    private NeonWeapon currentWeapon;

    public void EquipWeapon(GameObject weaponPrefab)
    {
        if (currentWeapon != null)
        {
            Destroy(currentWeapon.gameObject);
        }

        GameObject weaponObj = Instantiate(weaponPrefab, weaponSocket);
        currentWeapon = weaponObj.GetComponent<NeonWeapon>();
    }
}
```

### Accessing Weapon State

```csharp
var weapon = character.CurrentWeapon;
if (weapon != null)
{
    int ammo = weapon.CurrentAmmo;
    bool reloading = weapon.IsReloading;
}
```

## Important Notes

### Dependencies
- TextMeshPro for UI text rendering
- NavMesh Components for enemy AI navigation
- Physics system for raycasting

### Unity Version Compatibility
- Developed for Unity 2022.3 LTS
- Should work with Unity 2021.3+ with minimal changes
- May require updates for Unity 6.0+

### Performance Considerations
- Use object pooling for projectiles and effects
- Bake NavMesh for optimal AI performance
- Use LOD systems for distant objects
- Occlusion culling for large levels

## Extending the System

### Adding Abilities
- Create new `MonoBehaviour` for ability logic
- Use coroutines for cooldowns: `yield return new WaitForSeconds(cooldown)`
- Integrate with character input system
- Reference ability data from `MissionTypes.AscendantAbility`

### Creating Enemy AI
- Use `NavMeshAgent` for pathfinding
- Implement state machine for behaviors (patrol, chase, attack)
- Use `Physics.OverlapSphere` for detection
- Integrate weapon system when implemented

### Building Districts
- Model district geometry in preferred 3D tool
- Import to Unity and set up colliders
- Place NavMesh obstacles
- Add hazard spawners based on district data
- Configure lighting and post-processing

### UI/HUD Integration
- Use TextMeshPro for text rendering
- Create UI prefabs with `NeonHUD` component
- Bind to character and game manager
- Display mission brief on level start
- Show objective markers using world-space UI

## License

Original Unreal Engine project converted to Unity. All game design and lore content belongs to the original creators.

## Support

For issues with the Unity conversion, please check:
1. Unity version compatibility
2. Required packages installed (TextMeshPro, NavMesh)
3. Assembly definitions properly referenced
4. Prefabs correctly configured with components
