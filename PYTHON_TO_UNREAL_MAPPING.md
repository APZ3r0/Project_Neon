# Python to Unreal Engine Code Mapping

This document provides direct mappings from the Python prototype code to Unreal Engine C++/Blueprint implementations.

## Data Models

### `data.py` → Unreal Data Assets

#### Ability
```python
# Python
@dataclass(frozen=True)
class Ability:
    name: str
    description: str
    cooldown: int | None = None
    damage_type: str | None = None
```

```cpp
// Unreal C++
USTRUCT(BlueprintType)
struct FAbilityData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Name;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CooldownSeconds = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EAbilityDamageType DamageType = EAbilityDamageType::None;
};

UCLASS(BlueprintType)
class UAbilityDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FAbilityData AbilityData;
    
    // GAS Ability class reference
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class UGameplayAbility> AbilityClass;
};
```

#### Archetype
```python
# Python
@dataclass(frozen=True)
class Archetype:
    name: str
    role: str
    signature: str
    abilities: List[Ability]
```

```cpp
// Unreal C++
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
    FString Signature;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FArchetypeProfile BaseStats;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftObjectPtr<UAbilityDataAsset>> Abilities;
};
```

#### Weapon
```python
# Python
@dataclass(frozen=True)
class Weapon:
    name: str
    category: str
    description: str
    damage_profile: str
```

```cpp
// Unreal C++
UENUM(BlueprintType)
enum class EWeaponCategory : uint8
{
    Pistol,
    SMG,
    AssaultRifle,
    Railgun,
    PlasmaRifle,
    Experimental
};

UCLASS(BlueprintType)
class UWeaponDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString WeaponName;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EWeaponCategory Category;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DamageProfile;
    
    // Runtime generated
    UPROPERTY(BlueprintReadOnly)
    EWeaponRarity Rarity = EWeaponRarity::Common;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FWeaponAffix> Affixes;
};
```

#### District
```python
# Python
@dataclass(frozen=True)
class District:
    name: str
    description: str
    hazards: List[str]
    enemy_profiles: List[str]
```

```cpp
// Unreal C++
UCLASS(BlueprintType)
class UDistrictDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString DistrictName;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> Hazards;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> EnemyProfiles;
    
    // Procedural generation parameters
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UWorld> BaseLevel;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSoftObjectPtr<UClass>> ModularPieces;
};
```

### `build_codex()` → Codex Subsystem

```python
# Python
def build_codex() -> CodexDict:
    # Returns dict with all game data
    return {
        "abilities": [...],
        "archetypes": [...],
        "weapons": [...],
        # etc.
    }
```

```cpp
// Unreal C++
UCLASS()
class NEONASCENDANT_API UCodexSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
    
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    // Data accessors
    UFUNCTION(BlueprintCallable)
    TArray<UArchetypeDataAsset*> GetAllArchetypes() const;
    
    UFUNCTION(BlueprintCallable)
    UArchetypeDataAsset* GetArchetypeByName(const FString& Name) const;
    
    UFUNCTION(BlueprintCallable)
    TArray<UWeaponDataAsset*> GetAllWeapons() const;
    
    UFUNCTION(BlueprintCallable)
    TArray<UDistrictDataAsset*> GetAllDistricts() const;
    
    UFUNCTION(BlueprintCallable)
    TArray<UFactionDataAsset*> GetAllFactions() const;
    
private:
    UPROPERTY()
    TArray<UArchetypeDataAsset*> Archetypes;
    
    UPROPERTY()
    TArray<UWeaponDataAsset*> Weapons;
    
    UPROPERTY()
    TArray<UDistrictDataAsset*> Districts;
    
    UPROPERTY()
    TArray<UFactionDataAsset*> Factions;
    
    void LoadCodexData();
};
```

## Mission Generation

### `game.py` → Mission Generation Subsystem

```python
# Python
class MissionGenerator:
    def __init__(self, *, seed: int | None = None):
        self._codex = build_codex()
        self._random = random.Random(seed)
    
    def craft_brief(self) -> MissionBrief:
        district = self._choose(self._codex["districts"])
        opposition = self._choose(self._codex["factions"])
        # ...
```

```cpp
// Unreal C++
UCLASS()
class NEONASCENDANT_API UMissionGenerationSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable)
    FMissionBrief GenerateMissionBrief(int32 Seed = -1);
    
    UFUNCTION(BlueprintCallable)
    TArray<FMissionBrief> GenerateMissionBriefs(int32 Count, int32 Seed = -1);
    
private:
    UPROPERTY()
    UCodexSubsystem* CodexSubsystem;
    
    FRandomStream RandomStream;
    
    template<typename T>
    T* ChooseRandom(const TArray<T*>& Options);
    
    FString ChooseRandomComplication();
    FString ChooseRandomExtractionCondition();
};
```

### Constants Migration

```python
# Python
COMPLICATIONS: Sequence[str] = (
    "Ghost Grid instabilities cause random HUD distortion",
    # ...
)
```

```cpp
// Unreal C++
UCLASS()
class NEONASCENDANT_API UGameConstants : public UObject
{
    GENERATED_BODY()
    
public:
    static const TArray<FString> Complications;
    static const TArray<FString> ExtractionConditions;
    
    static const int32 AbilitySynergyBonus = 2;
    static const int32 ImplantSynergyBonus = 1;
    static const int32 DiceRollMin = 1;
    static const int32 DiceRollMax = 6;
    static const int32 DifficultyVarianceMin = 0;
    static const int32 DifficultyVarianceMax = 3;
};

// In .cpp file
const TArray<FString> UGameConstants::Complications = {
    TEXT("Ghost Grid instabilities cause random HUD distortion"),
    TEXT("Helix orbital overwatch sweeps disrupt cloak cycles"),
    TEXT("Dawnbreaker converts attempt to hack your cyberdeck mid-fight"),
    TEXT("Rival Ascendant strike team is pursuing the same data ghost")
};
```

## Simulation System

### `simulation.py` → Simulation Subsystem

```python
# Python
class SimulationEngine:
    def simulate(
        self,
        *,
        archetype: str | None = None,
        district: str | None = None,
        difficulty: int = 6,
    ) -> MissionReport:
```

```cpp
// Unreal C++
USTRUCT(BlueprintType)
struct FStageResult
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString StageName;
    
    UPROPERTY(BlueprintReadOnly)
    FString Challenge;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Roll = 0;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Threshold = 0;
    
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess = false;
    
    UPROPERTY(BlueprintReadOnly)
    FString Narration;
};

USTRUCT(BlueprintType)
struct FMissionReport
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FMissionBrief Brief;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FStageResult> Stages;
    
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess = false;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Rewards;
};

UCLASS()
class NEONASCENDANT_API USimulationSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable)
    FMissionReport SimulateMission(
        const FString& ArchetypeName = TEXT(""),
        const FString& DistrictName = TEXT(""),
        int32 Difficulty = 6,
        int32 Seed = -1
    );
    
private:
    FArchetypeProfile GetArchetypeProfile(const FString& ArchetypeName);
    FString GetAbilitySynergyStat(const FString& AbilityName);
    FString GetImplantSynergyStat(const FString& ImplantSlot);
    
    TArray<FStageDefinition> MissionStages;
};
```

## Archetype Profiles

```python
# Python
_ARCHETYPE_PROFILES = {
    "Specter": {"stealth": 9, "assault": 5, "tech": 8, "resilience": 4},
    # ...
}
```

```cpp
// Unreal C++
// Store in ArchetypeDataAsset instead of hardcoded
// Or use a data table for runtime lookup

USTRUCT(BlueprintType)
struct FArchetypeProfileTableRow : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString ArchetypeName;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FArchetypeProfile Profile;
};

// In CodexSubsystem
UPROPERTY()
UDataTable* ArchetypeProfileTable;
```

## Utility Functions

### Random Choice

```python
# Python
def _choose(self, options: Iterable[T]) -> T:
    return self._random.choice(tuple(options))
```

```cpp
// Unreal C++
template<typename T>
T* UMissionGenerationSubsystem::ChooseRandom(const TArray<T*>& Options)
{
    if (Options.Num() == 0)
    {
        return nullptr;
    }
    
    int32 Index = RandomStream.RandRange(0, Options.Num() - 1);
    return Options[Index];
}
```

### Name Lookup

```python
# Python
def _name_lookup(
    items: Iterable[T],
    requested: str | None,
    *,
    item_type: str,
    rng: random.Random,
) -> T:
```

```cpp
// Unreal C++
template<typename T>
T* UCodexSubsystem::FindByName(const TArray<T*>& Items, const FString& Name) const
{
    if (Name.IsEmpty())
    {
        return nullptr;
    }
    
    for (T* Item : Items)
    {
        if (Item && Item->GetName().Equals(Name, ESearchCase::IgnoreCase))
        {
            return Item;
        }
    }
    
    return nullptr;
}
```

## Data Import Workflow

### Step 1: Export Python Data to JSON/CSV

```python
# Create export script
import json
from neon_ascendant.data import build_codex

codex = build_codex()

# Export to JSON
with open("codex_export.json", "w") as f:
    json.dump({
        "archetypes": [
            {
                "name": arch.name,
                "role": arch.role,
                "signature": arch.signature,
                "abilities": [a.name for a in arch.abilities]
            }
            for arch in codex["archetypes"]
        ],
        # ... etc
    }, f, indent=2)
```

### Step 2: Import into Unreal Data Tables

1. Create CSV files from JSON export
2. Import CSV into Unreal Data Tables
3. Create Data Asset Blueprints that reference table rows
4. Load into CodexSubsystem on game start

### Step 3: Runtime Access

```cpp
// C++ access
UCodexSubsystem* Codex = GetGameInstance()->GetSubsystem<UCodexSubsystem>();
UArchetypeDataAsset* Specter = Codex->GetArchetypeByName(TEXT("Specter"));
```

```blueprint
// Blueprint access
Get Game Instance -> Get Subsystem (Codex Subsystem) -> Get Archetype By Name
```

## Testing Strategy

### Unit Tests

```python
# Python pytest
def test_craft_briefs_respects_count():
    generator = MissionGenerator(seed=42)
    briefs = generator.craft_briefs(2)
    assert len(briefs) == 2
```

```cpp
// Unreal Automation Tests
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FMissionGenerationTest,
    "NeonAscendant.MissionGeneration.Count",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter
)

bool FMissionGenerationTest::RunTest(const FString& Parameters)
{
    UMissionGenerationSubsystem* Subsystem = NewObject<UMissionGenerationSubsystem>();
    TArray<FMissionBrief> Briefs = Subsystem->GenerateMissionBriefs(2, 42);
    
    TestEqual(TEXT("Should generate 2 briefs"), Briefs.Num(), 2);
    
    return true;
}
```

## Performance Considerations

### Python (Prototype)
- Runtime generation: ~1ms per mission brief
- No memory concerns (small data)

### Unreal (Production)
- **Data Loading**: Load all data assets at startup (one-time cost)
- **Mission Generation**: Cache codex data, use FRandomStream (deterministic)
- **Memory**: Use soft references (`TSoftObjectPtr`) to avoid loading unused assets
- **Async Loading**: Load district levels asynchronously when mission starts

## Migration Checklist

- [ ] Port all data structures to C++ structs
- [ ] Create Data Asset classes
- [ ] Export Python data to CSV/JSON
- [ ] Import into Unreal Data Tables
- [ ] Implement CodexSubsystem
- [ ] Port MissionGenerator to C++
- [ ] Port SimulationEngine to C++
- [ ] Create unit tests for mission generation
- [ ] Verify deterministic behavior with seeds
- [ ] Performance test data loading
- [ ] Document Blueprint APIs

---

**Note:** This mapping focuses on the core data and generation systems. Gameplay systems (combat, abilities, etc.) will require additional Unreal-specific implementations using GAS, weapon systems, and character controllers.

