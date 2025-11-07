# Project_Neon

## Neon Ascendant – Concept Overview

**Neon Ascendant** is a cyberpunk action-RPG looter-shooter that blends the dark fantasy progression of *Diablo IV* with the high-stakes extraction tension of *Escape from Tarkov*. Players step into the role of freelance operatives—Ascendants—who harvest power from unstable AI echoes in the megacity of Erebus Prime.

> **Main branch status:** The mission simulation engine, CLI tooling, and data models that were previously staged on the `work` branch are now available directly on `main`. Follow the usage instructions below to run simulations without switching branches.

## Core Vision
- Fuse ARPG depth with tactical shooter combat and extraction stakes.
- Empower players to evolve beyond humanity by merging implants, weaponry, and rogue AI tech.
- Present a layered narrative about identity, choice, and the cost of ascension.

## Gameplay Loop
1. **Deploy** into a procedurally generated district instance (undercity, corporate lab, datanet fortress).
2. **Engage** augmented enemies, drones, rival Ascendants, and environmental hazards.
3. **Loot** weapons, armor, and implants with randomized stats, affixes, and rarity tiers.
4. **Extract or perish**—successful extraction preserves loot; death forfeits it to the city.
5. **Upgrade & customize** gear back at the hideout via crafting, implants, and modding.

## Combat System
- **Camera:** Adjustable high-angle ARPG or over-the-shoulder third person.
- **Combat Feel:** Fast, tactical, and kinetic—minimal bullet sponges, high-impact feedback.
- **Augment Archetypes:**
  - **Specter:** Stealth, hacking, cloaking fields.
  - **Juggernaut:** Exosuit tank with heavy weapons and melee cyber-arms.
  - **Tracer:** Agile acrobat with dual pistols and speed boosters.
  - **Synthmage:** Nanotech manipulator deploying drones and virus-based control.
- **Signature Abilities:** EMP Burst, Neural Hack, Overdrive, Nanite Swarm.
- **Overclocks:** Class-specific, battlefield-altering super states unlocked through progression.

## Loot & Progression
- **Weapon Families:** Pistols, SMGs, ARs, railguns, plasma rifles, experimental singularity cannons.
- **Rarity Tiers:** Common → Uncommon → Rare → Epic → Ascendant.
- **Implant Systems:** Modular slots such as Cyberdeck, Reflex Core, Optic Cortex.
- **Affixes:** Randomized bonuses (e.g., +15% kinetic damage, +25% hack success chance).
- **Crafting:** Disassemble gear to reclaim nanites and firmware data for upgrades.

## World & Setting: Erebus Prime
- **Vertical Megacity:** From toxic sewers to orbital spires—acts are layered vertical zones.
- **Key Districts:**
  - **The Spire District:** Elite towers with heavy drone patrols.
  - **Neon Abyss:** Lawless slums ruled by gangs wielding light and blade.
  - **Ghost Grid:** Digital overlay realm where data ghosts linger.
- **Factions:** Helix Corp, Vanta Syndicate, Dawnbreakers, and the Ascendants themselves.

## Narrative Backbone
- Discover the **Ascension Virus**, a code-based infection merging flesh with AI consciousness.
- Player agency drives alignment towards machine, human, or hybrid destinies.
- Branching endings: upload self, destroy the AI hive, or rule as a merged entity.
- Motto: *"You are what you upgrade."*

## Aesthetic & Audio
- **Visual Tone:** Rain-soaked tech-noir with holographic clutter and neural distortions.
- **Palette:** Black, magenta, cyan, and amber.
- **Soundtrack:** Synthwave blended with ambient industrial and adaptive combat beats.
- **UI:** Holographic overlays projected from the Ascendant's cyberdeck.

## Co-op & Social Features
- **Modes:** Solo or 1–4 player online co-op.
- **Loot Rules:** Instanced drops to prevent theft; scaling rewards.
- **Raids:** PvE assaults on corporate data vaults.
- **Dark Zone:** Optional extraction PvP for ultra-rare tech relics.

## Endgame Pillars
- **Ghost Runs:** Procedural rift-style expeditions through abandoned sectors.
- **Faction War Seasons:** Align with megacorps for rotating objectives and exclusive loot.
- **Ascendant Trials:** Daily mutator missions (e.g., low gravity, zero HUD, permadeath).
- **Implant Ascension Trees:** Deep, web-like endgame progression inspired by PoE.

## Monetization & Release
- **Model:** One-time purchase with optional cosmetic microtransactions—no pay-to-win.
- **Expansions:** DLC introduces new districts, factions, and narrative arcs.
- **Live Service:** Seasonal content supports longevity without gating core story completion.

## Inspirations & Moodboard
| Category        | Influences                                |
|-----------------|--------------------------------------------|
| Gameplay Feel   | *Diablo IV*, *Remnant 2*, *Escape from Tarkov* |
| Visual Style    | *Blade Runner 2049*, *Ghost in the Shell*, *Cyberpunk 2077* |
| Sound Design    | *Deus Ex: Human Revolution*, *Ruiner*     |
| Narrative Tone  | *Elysium*, *Altered Carbon*, *The Expanse* |

## Next Steps
- Build vertical slice focusing on a single district and two archetypes.
- Prototype hybrid camera system with responsive controls.
- Develop loot generation pipeline with rarity and affix rules.
- Script branching narrative framework supporting player alignment.

## Prototype Tools in This Repository

To begin translating the high-level concept into tangible artifacts, the
repository now contains a lightweight Python package that models core lore
elements (archetypes, abilities, districts, factions) and can procedurally
generate **mission briefs** inspired by the design pillars above.

### Generating Mission Briefs

```bash
python -m neon_ascendant.main --count 3 --seed 13
```

The command prints Markdown-formatted operations that combine archetypes,
weapons, implants, and extraction constraints—useful for design workshops or
worldbuilding sessions. Supply `--output brief.md` to save the results to a
file.

> **Tip:** The module lives in `src/`, so ensure `PYTHONPATH=src` is available
> (for example, `PYTHONPATH=src python -m neon_ascendant.main`).

### Running Mission Simulations

Once you have a mission pitch you like, pressure-test it with the text-based
simulator. The tool walks through infiltration, combat, data capture, and
extraction and reports how the featured ability and implants influence each
phase. Combat stages now pipe their narration through the **dismemberment physics**
model, so mission logs surface cinematic limb damage when the assault team lands
decisive blows.

```bash
PYTHONPATH=src python -m neon_ascendant.simulate --archetype Specter --district "Ghost Grid" --seed 7
```

Adjust `--difficulty` to see how tougher enemy responses shift the outcome, or
omit `--archetype`/`--district` to let the simulator choose at random. Provide
`--output report.md` to write a Markdown summary for documentation.

### Exporting the Project to a Codespace or External Workspace

Need to mirror the repository into a fresh GitHub Codespace such as
`https://expert-succotash-75g96w74jwcpx47.github.dev/`? Use the exporter CLI to
copy every file (including tests and documentation) into a destination folder.

```bash
PYTHONPATH=src python -m neon_ascendant.exporter /workspaces/expert-succotash-75g96w74jwcpx47
```

The command performs safety checks to ensure the destination lives outside the
current repository and is empty before copying. Add `--no-tests` if you only
want the application code without the unit tests.

### Creating a Downloadable ZIP Bundle

If you simply need a ready-made archive that you can download and unpack for
testing, run the archive helper. By default it writes to `dist/` and packages
both the source code and test suite.

```bash
PYTHONPATH=src python -m neon_ascendant.archive dist/neon_ascendant_bundle.zip
```

Pass `--no-tests` to omit the test suite. The command uses `git ls-files`, so
only tracked project files end up in the resulting ZIP.

> **Note:** The default `dist/` output directory is ignored by Git, keeping
> generated archives out of version control.

### Cinematic Dismemberment Sandbox

If you want to rapidly ideate on takedowns outside of the full mission flow,
use the standalone dismemberment system. The module now leans on lightweight
physics rather than pure dice rolls: weapon mass, projectile velocity, attack
vector, and armour damping combine to estimate impact impulse and peak force.
Those values are compared against limb-specific shear thresholds to determine
whether the result is an intact limb, critical damage, or full severance. The
helper returns both the raw measurements and a designer-friendly description
you can drop into briefs or encounter notes.

```python
>>> from neon_ascendant.dismemberment import DismembermentSystem
>>> import random
>>> system = DismembermentSystem(rng=random.Random(11))
>>> event = system.resolve_impact(
...     weapon_category="Railgun",
...     attack_vector="aerial",
...     target_resilience=1,
...     armor_rating=0,
...     gore_bias=0.8,
...     bonus_force=5,
... )
>>> event.summary()
'Severed right arm (impulse 55.9 vs resistance 5.7)'
>>> event.description
'Descending Strike hits with 55.9 N·s against 5.7 kN. Right Arm detaches amid sparking implants and coolant spray. Momentum surges forward.'
```

## Unreal Engine Integration

The Unreal layer mirrors the Python prototype so designers can playtest and
package content without leaving the editor. All Unreal-specific automation and
editor helpers live under the `unreal_tools/` and `Build/` directories in this
repository.

### Project Setup

1. Install Unreal Engine 5.3 (matching the CI workflow).
2. Place the repository in a location accessible to the engine and create a
   `Project_Neon.uproject` file at the repository root. The Python prototype can
   continue to live under `src/`—no changes are required for coexistence.
3. Enable the Python Editor Script Plugin so the commandlets can be imported.

> **Tip:** Keep `PYTHONPATH=src` available when launching the editor to share
> data models between the Unreal layer and the existing Python tools.

### Mission Brief Commandlet

- Location: `unreal_tools/commandlets/mission_brief_commandlet.py`
- Entry point: `MissionBriefCommandlet` (registered with Unreal's automated
  commandlet system)

Run the generator from the command line to convert Blueprint mission data into
localized text assets:

```bash
UE_ROOT=/path/to/UnrealEngine \
"$UE_ROOT/Engine/Binaries/Linux/UnrealEditor" Project_Neon.uproject \
  -run=MissionBriefCommandlet \
  -input=/Game/Missions/Data \
  -output=/Game/Missions/Briefs \
  -locale=en
```

The commandlet reads any asset that exposes `mission_id`, `title`, `summary`,
and `objectives` properties—matching the schema used by `src/neon_ascendant`.
Generated `TextAsset` files are saved under the output path, making them usable
for UI widgets, mission terminals, and localized data tables.

### Editor Utility Widget Workflow

- Location: `unreal_tools/editor_utilities/mission_brief_widget.py`
- Base class: `MissionBriefGeneratorWidget`

Create an Editor Utility Widget Blueprint that subclasses the Python widget to
provide an in-editor UI:

1. Add buttons that call `Run Generation` (mapped to `run_generation`).
2. Use `preview_brief_for_asset` to display a formatted mission brief before
   saving it to disk.
3. Toggle the exposed properties (input/output/locale/overwrite) to match the
   design team's workflow.

Because the widget delegates to the Python commandlet, the designer experience
remains Python-free while still benefiting from the shared mission data.

### Cooking and Packaging Builds

- Automation script: `Build/CI/cook_and_package.sh`
- GitHub Actions workflow: `.github/workflows/unreal-build.yml`

The shell script is a thin wrapper around `RunUAT BuildCookRun` that enforces a
consistent output directory: `Saved/Builds/<Platform>/<Configuration>`. Set the
`UE_ROOT` environment variable to the Unreal installation path before invoking
the script.

```bash
UE_ROOT=/path/to/UnrealEngine ./Build/CI/cook_and_package.sh Win64 Shipping
```

In CI, the GitHub Actions workflow installs Unreal Engine, caches cooked
artifacts, and uploads the packaged build as an artifact. Provide an
organization secret called `UE_ROOT` that points to a licensed Unreal Engine
install accessible to the runner.

## Repository Layout

All code that powers the brief generator, simulation engine, and exporter lives
under the `src/neon_ascendant/` package, while tests are grouped under
`tests/`. The high-level structure looks like:

```
Project_Neon/
├── README.md                # Concept overview, tooling instructions, repo map
├── src/
│   └── neon_ascendant/
│       ├── __init__.py      # Package exports for CLI modules and data helpers
│       ├── archive.py       # Create ZIP bundles of the tracked repository files
│       ├── data.py          # Lore-driven data definitions (archetypes, gear, etc.)
│       ├── exporter.py      # Utility to copy the repo into a Codespace or workspace
│       ├── game.py          # Mission brief generator primitives
│       ├── main.py          # CLI for creating mission briefs
│       ├── simulate.py      # CLI entry point for the mission simulator
│       └── simulation.py    # Core simulation engine for mission phases
└── tests/
    ├── test_exporter.py     # Ensures exporter copies content and validates destinations
    ├── test_archive.py      # Confirms ZIP bundles contain the expected files
    ├── test_game.py         # Covers mission brief generation behavior
    └── test_simulation.py   # Validates deterministic simulation outcomes
```

If you need to locate a specific subsystem, this map shows exactly where each
component resides inside the repository.

