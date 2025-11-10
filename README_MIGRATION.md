# Unreal Engine Migration - Quick Reference

This document provides a quick overview of the migration resources and how to use them.

## 📚 Documentation Files

1. **UNREAL_MIGRATION_PLAN.md** - Complete 40-week migration strategy
   - Architecture design
   - Phase-by-phase implementation plan
   - Risk assessment
   - Team requirements

2. **PYTHON_TO_UNREAL_MAPPING.md** - Technical code translations
   - Direct Python → C++ mappings
   - Data structure conversions
   - Implementation examples

3. **QUICK_START_GUIDE.md** - Step-by-step getting started guide
   - Day-by-day setup instructions
   - Common issues and solutions
   - First week implementation plan

## 🛠️ Tools & Scripts

### Data Export Script

**Location:** `scripts/export_to_unreal.py`

**Usage:**
```bash
cd Project_Neon-main
python scripts/export_to_unreal.py
```

**Output:**
- `unreal_data/codex_export.json` - Master JSON with all game data
- `unreal_data/Archetypes.csv` - CSV for Unreal Data Table import
- `unreal_data/Weapons.csv` - Weapon data CSV
- `unreal_data/Districts.csv` - District data CSV
- `unreal_data/Factions.csv` - Faction data CSV
- `unreal_data/GameConstants.h` - C++ constants header

**What it does:**
- Exports all Python codex data to Unreal-compatible formats
- Includes archetype stats from simulation system
- Generates CSV files for Data Table import
- Creates C++ constants header file

## 📦 Code Templates

**Location:** `unreal_templates/`

### Available Templates

1. **ArchetypeData.h** - Archetype data structure
   - `FArchetypeProfile` struct
   - `UArchetypeDataAsset` class
   - Data table row structure

2. **CodexSubsystem.h/.cpp** - Central data access system
   - Replaces Python `build_codex()` function
   - Provides Blueprint/C++ APIs for data access
   - Loads all game data on startup

3. **MissionGenerationSubsystem.h** - Mission brief generation
   - Ported from Python `MissionGenerator`
   - Deterministic seed-based generation
   - Blueprint-callable functions

### How to Use Templates

1. Copy template files to your Unreal project:
   ```
   unreal_templates/ArchetypeData.h → Source/NeonAscendantCore/Public/Data/
   unreal_templates/CodexSubsystem.h → Source/NeonAscendantCore/Public/Subsystems/
   unreal_templates/CodexSubsystem.cpp → Source/NeonAscendantCore/Private/Subsystems/
   ```

2. Update includes and module names to match your project

3. Implement missing data asset classes (WeaponData, DistrictData, etc.)

4. Compile and test

## 🚀 Getting Started Workflow

### Step 1: Export Data
```bash
python scripts/export_to_unreal.py
```

### Step 2: Set Up Unreal Project
Follow `QUICK_START_GUIDE.md` Day 1-2 instructions

### Step 3: Import Data
1. Import CSV files into Unreal Data Tables
2. Create Data Assets from exported JSON
3. Copy `GameConstants.h` to your project

### Step 4: Implement Core Systems
1. Copy code templates to project
2. Implement missing data asset classes
3. Set up CodexSubsystem
4. Test data access

### Step 5: Port Mission Generation
1. Implement MissionGenerationSubsystem
2. Test mission brief generation
3. Verify deterministic behavior with seeds

## 📋 Migration Checklist

### Phase 1: Foundation (Weeks 1-4)
- [ ] Export Python data to Unreal formats
- [ ] Set up Unreal project structure
- [ ] Create C++ modules
- [ ] Port data models (structs, enums)
- [ ] Implement CodexSubsystem
- [ ] Create data assets
- [ ] Import data from CSV/JSON
- [ ] Basic character controller

### Phase 2: Core Gameplay (Weeks 5-10)
- [ ] Weapon system
- [ ] Ability system (GAS)
- [ ] Stat system
- [ ] Archetype system
- [ ] Basic AI enemies
- [ ] Combat loop

### Phase 3: Loot & Progression (Weeks 11-14)
- [ ] Loot generation
- [ ] Rarity/affix system
- [ ] Inventory system
- [ ] Extraction validation

### Phase 4: Procedural Generation (Weeks 15-20)
- [ ] District generation
- [ ] Mission brief generation (port from Python)
- [ ] Objective system
- [ ] Extraction zones

### Phase 5: Multiplayer (Weeks 21-26)
- [ ] Networking setup
- [ ] Character replication
- [ ] Matchmaking
- [ ] Co-op functionality

### Phase 6: Polish (Weeks 27-32)
- [ ] UI/UX polish
- [ ] Audio integration
- [ ] VFX
- [ ] Balancing

### Phase 7: Endgame (Weeks 33-40)
- [ ] Ghost Runs
- [ ] Faction Wars
- [ ] Ascendant Trials
- [ ] Launch prep

## 🔗 Key Resources

- **Unreal Documentation**: https://docs.unrealengine.com/
- **Gameplay Ability System**: https://github.com/tranek/GASDocumentation
- **C++ Programming Guide**: https://docs.unrealengine.com/en-US/programming-and-scripting/programming-with-cpp/
- **Data Assets**: https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/Assets/DataAssets/

## 💡 Tips

1. **Start Small**: Get data systems working before gameplay
2. **Test Early**: Verify mission generation matches Python output
3. **Use Seeds**: Deterministic generation helps with testing
4. **Iterate**: Don't try to port everything at once
5. **Reference Python**: Keep Python code open for reference

## 🐛 Common Issues

### Data not loading
- Check Asset Manager configuration
- Verify Primary Asset Types are set
- Ensure data assets are in correct folders

### Compilation errors
- Check module dependencies in `.Build.cs`
- Verify all includes are correct
- Clean and rebuild solution

### Mission generation differs from Python
- Check random seed initialization
- Verify data matches Python codex
- Test with same seed values

## 📞 Next Steps

1. Review `UNREAL_MIGRATION_PLAN.md` for full strategy
2. Follow `QUICK_START_GUIDE.md` for immediate setup
3. Use `PYTHON_TO_UNREAL_MAPPING.md` for code translations
4. Run export script to generate data files
5. Copy templates to your Unreal project

---

**Last Updated:** 2024  
**Version:** 1.0

