# Neon Ascendant: Copilot Instructions

## Project Overview

**Neon Ascendant** is a sci-fi tactical mission generator being migrated from Python prototype to Unreal Engine 5.3+. The codebase contains two distinct parts: a complete Unreal C++ runtime module and supporting Python infrastructure for testing and tooling.

### Core Purpose
- **Python**: Lore data structures, mission generation logic, test validation
- **C++**: Blueprint-friendly mission generator for Unreal Engine gameplay
- **Deterministic Generation**: All missions seeded by `FRandomStream` for reproducibility

---

## Architecture: Python ↔ Unreal Migration

### Two-Layer Design
1. **Python Prototype Layer** (`src/neon_ascendant/`)
   - Original game design and data definitions
   - Lore constants (districts, factions, weapons, implants, archetypes)
   - Mission generation algorithm reference
   - Test suite validates C++ implementation matches Python behavior

2. **Unreal Runtime Layer** (`Source/NeonAscendant/`)
   - Hard-coded C++ equivalents of all lore data
   - Blueprint-callable mission generator API
   - Singleton pattern for safe Blueprint access
   - Uses `FRandomStream` for deterministic seeding

### Key Data Structures (C++)
All defined in `MissionTypes.h`:
- `FAscendantArchetype`: Character class (name, role, signature, abilities)
- `FAscendantWeapon`: Combat equipment
- `FAscendantImplant`: Enhancement slots with effects array
- `FAscendantDistrict`: Mission location with hazards/enemies
- `FAscendantFaction`: Opposition faction with tactics
- `FMissionBrief`: Complete generated mission (all above + complication + extraction)

---

## Critical Developer Workflows

### Testing & Validation

```bash
# Install test dependencies
python -m pip install -e .[test]

# Run full test suite (validates Unreal C++ matches Python logic)
pytest

# Specific test categories
pytest tests/test_unreal_project.py  # C++ structure validation
pytest tests/test_dismemberment.py   # Lore data consistency
```

**Test Philosophy**: Python tests assert that C++ source code contains all lore data and exposes correct Blueprint APIs. This is the primary validation that migration is complete.

### Building the Unreal Module

The module is self-contained in `Source/NeonAscendant/`. When you open `.uproject` in Unreal:
1. Editor prompts to compile the `NeonAscendant` runtime module
2. Module builds with dependencies: `Core`, `CoreUObject`, `Engine`, `GameplayTags`
3. All Blueprint APIs become available immediately

### Seeding Missions for Determinism

```cpp
// In Blueprint or C++
UMissionGenerator* Gen = UMissionGeneratorSingleton::GetGenerator();
Gen->SeedGenerator(1337);  // Deterministic: always generates same missions

TArray<FMissionBrief> Briefs;
Gen->GenerateMissionBriefs(5, Briefs);  // 5 unique briefs from seed
```

This is critical for documentation, automated testing, and reproducible design feedback.

---

## Project Conventions & Patterns

### Naming & Data Organization

- **Archetype Constants**: Hardcoded in `MissionData.cpp` under `NeonAscendantData::GetArchetypes()`
- **Blueprint Exposure**: Use `UCLASS(BlueprintType)` and `UPROPERTY(BlueprintReadOnly)` with category tags
- **Mission Brief Assembly**: No database—random selection from in-memory arrays using `RandomStream.RandRange()`
- **Singleton Pattern**: `UMissionGeneratorSingleton::GetGenerator()` prevents multiple instances; safe for Blueprint

### C++ Structure for Lore Data

```cpp
// In MissionData.cpp, all constants follow this pattern:
namespace NeonAscendantData
{
    const TArray<FAscendantArchetype>& GetArchetypes() 
    {
        static const TArray<FAscendantArchetype> Archetypes = {
            FAscendantArchetype{
                TEXT("Specter"),
                TEXT("Stealth infiltrator"),
                TEXT("Adaptive camouflage and ghosting protocols"),
                { /* abilities array */ }
            },
            // ... more archetypes
        };
        return Archetypes;
    }
}
```

This pattern ensures data is loaded once and cached.

### Random Generation Philosophy

- Use `FRandomStream` (Unreal's seeded RNG), NOT global `rand()`
- Always pass the stream by reference to helper functions
- Seed before generating multiple briefs: `RandomStream.Initialize(Seed)`
- Index selection: `RandomStream.RandRange(0, Array.Num() - 1)`

---

## Integration Points & Dependencies

### Unreal External Dependencies
- `GameplayTags`: Used for archetype identification
- No gameplay ability system (GAS) in base module; designed for easy extension
- No data tables yet—all data is C++ structs (can migrate to Data Assets later)

### Python ↔ C++ Validation
The test suite (`tests/test_unreal_project.py`) checks:
1. `.uproject` declares `NeonAscendant` as Runtime module
2. Build file links all required dependencies
3. `MissionGenerator.h` exposes: `GenerateMissionBrief()`, `GenerateMissionBriefs()`, `SeedGenerator()`
4. `MissionData.cpp` contains all original lore strings (e.g., "Specter", "Pulsecaster SMG")
5. Complications and extraction conditions are defined as string arrays

**Before committing C++ changes**, run `pytest tests/test_unreal_project.py::test_mission_generator_exposes_blueprint_api` to validate the API contract.

---

## Common Task Patterns

### Adding a New Archetype
1. Add new `FAscendantArchetype` entry to `MissionData.cpp::GetArchetypes()`
2. Include name, role, signature, and at least 2 signature abilities
3. Add archetype name to Python `src/neon_ascendant/data.py` (keep in sync)
4. Run `pytest` to verify it appears in C++ source
5. Recompile Unreal module; new archetype auto-included in random selection

### Adding Mission Complication
1. Add string to `NeonAscendantData::GetComplications()` array
2. Add to Python `data.py`
3. Run tests; recompile

### Debugging Mission Generation
1. In Blueprint, create actor with mission generator component
2. Call `SeedGenerator(1337)` to get reproducible output
3. Call `GenerateMissionBrief()`
4. Print mission to log: all fields are Blueprint-readable strings/structs
5. Compare with Python generation using same seed to identify divergences

---

## File Reference Guide

| File | Purpose |
|------|---------|
| `Source/NeonAscendant/Public/MissionGenerator.h` | Blueprint API for mission generation |
| `Source/NeonAscendant/Public/MissionTypes.h` | All USTRUCT definitions (archetypes, weapons, etc.) |
| `Source/NeonAscendant/Public/MissionData.h` | Namespace functions returning lore data arrays |
| `Source/NeonAscendant/Private/MissionData.cpp` | **Actual lore data (archetypes, weapons, districts, factions, complications)** |
| `Source/NeonAscendant/Private/MissionGenerator.cpp` | Core random selection and brief assembly logic |
| `tests/test_unreal_project.py` | **Validation that C++ matches Python contract** |
| `PYTHON_TO_UNREAL_MAPPING.md` | Detailed code translation examples |
| `UNREAL_MIGRATION_PLAN.md` | Full architecture and phase roadmap |

---

## Workflow Checklist for Changes

- [ ] Python change? Update both `src/neon_ascendant/` and `Source/NeonAscendant/`
- [ ] New C++ struct? Add to `MissionTypes.h` AND update `MissionData.cpp`
- [ ] Modified lore data? Verify pytest passes `test_data_sets_include_all_original_concepts`
- [ ] Changed mission generation logic? Test with fixed seed (e.g., `1337`) to compare Python vs C++
- [ ] Blueprint API change? Ensure tests verify `UCLASS(BlueprintType)` markers are present

