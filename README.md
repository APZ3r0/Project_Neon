# Neon Ascendant – Concept Overview

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
phase.

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
│       ├── data.py          # Lore-driven data definitions (archetypes, gear, etc.)
│       ├── exporter.py      # Utility to copy the repo into a Codespace or workspace
│       ├── game.py          # Mission brief generator primitives
│       ├── main.py          # CLI for creating mission briefs
│       ├── simulate.py      # CLI entry point for the mission simulator
│       └── simulation.py    # Core simulation engine for mission phases
└── tests/
    ├── test_exporter.py     # Ensures exporter copies content and validates destinations
    ├── test_game.py         # Covers mission brief generation behavior
    └── test_simulation.py   # Validates deterministic simulation outcomes
```

If you need to locate a specific subsystem, this map shows exactly where each
component resides inside the repository.

