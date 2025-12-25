# Project_Neon

## Neon Ascendant – Unreal Engine Prototype

The **Neon Ascendant** initiative has been migrated from the original Python
lore tools into an Unreal Engine runtime module. The repository now ships a
self-contained C++ mission generator that recreates the faction, district,
weapon, implant, and archetype data from the earlier concept bible while making
it usable inside Unreal Engine 5 blueprints or C++ gameplay systems.

- **Engine Target:** Unreal Engine 5.3 (the `.uproject` may be upgraded by the
  editor if you use a newer release).
- **Module Name:** `NeonAscendant` (runtime module with Blueprint-friendly
  APIs).
- **Feature Highlight:** Deterministic mission briefs seeded by `FRandomStream`
  that combine codex data, complications, and extraction conditions.

## Repository Layout

```
Config/                         Project configuration defaults
NeonAscendant.uproject          Unreal project descriptor
Source/NeonAscendant/           Runtime module with gameplay data and logic
  ├── NeonAscendant.Build.cs    Build script with engine dependencies
  ├── Public/
  │     ├── MissionData.h       Accessors for codex data tables
  │     ├── MissionGenerator.h  Blueprint-facing mission generator API
  │     ├── MissionTypes.h      Shared USTRUCT definitions
  │     └── NeonAscendant.h     Module interface
  └── Private/
        ├── MissionData.cpp     Hard-coded lore data migrated from Python
        ├── MissionGenerator.cpp Mission assembly logic and singleton
        └── NeonAscendant.cpp   Module bootstrap
```

The original Python scripts have been retired in favour of native Unreal code.
All lore terms, mission complications, and extraction conditions are preserved
inside `MissionData.cpp` and surfaced through Blueprint-compatible structures.

## Using the Mission Generator in Unreal

1. Clone the repository and open `NeonAscendant.uproject` in Unreal Engine 5.3
   or later. The editor will prompt to build the `NeonAscendant` module.
2. Add a Blueprint (for example, an `Actor`) and include the mission generator:
   ```blueprint
   Get Mission Generator Singleton → Generate Mission Brief
   ```
3. The returned `FMissionBrief` struct includes the district, opposing faction,
   featured archetype, weapon, implant, complication, and extraction condition.
4. Call `SeedGenerator` before generating briefs to obtain deterministic
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
