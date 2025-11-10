"""Lightweight text simulation for Neon Ascendant extraction missions."""

from __future__ import annotations

import random
from dataclasses import dataclass
from typing import Iterable, List, Sequence, TypeVar

T = TypeVar("T")

from .data import (
    COMPLICATIONS,
    EXTRACTION_CONDITIONS,
    Archetype,
    District,
    Implant,
    Weapon,
    build_codex,
)
from .game import MissionBrief, MissionGenerator


@dataclass(frozen=True)
class StageDefinition:
    """Describe a phase of the mission along with the primary stat it stresses."""

    name: str
    challenge: str
    focus_stat: str


@dataclass(frozen=True)
class StageResult:
    """Outcome from a single stage of the simulated mission."""

    name: str
    challenge: str
    roll: int
    threshold: int
    success: bool
    narration: str


@dataclass(frozen=True)
class MissionReport:
    """Narrative summary of a complete mission simulation."""

    brief: MissionBrief
    stages: Sequence[StageResult]
    success: bool
    rewards: Sequence[str]


# Simulation constants
ABILITY_SYNERGY_BONUS = 2  # Bonus to roll when ability matches stage focus
IMPLANT_SYNERGY_BONUS = 1  # Bonus to roll when implant matches stage focus
DICE_ROLL_MIN = 1  # Minimum dice roll value
DICE_ROLL_MAX = 6  # Maximum dice roll value
DIFFICULTY_VARIANCE_MIN = 0  # Minimum difficulty variance
DIFFICULTY_VARIANCE_MAX = 3  # Maximum difficulty variance

_ARCHETYPE_PROFILES = {
    "Specter": {"stealth": 9, "assault": 5, "tech": 8, "resilience": 4},
    "Juggernaut": {"stealth": 4, "assault": 9, "tech": 5, "resilience": 8},
    "Tracer": {"stealth": 7, "assault": 8, "tech": 5, "resilience": 6},
    "Synthmage": {"stealth": 5, "assault": 4, "tech": 9, "resilience": 7},
}

_ABILITY_SYNERGY = {
    "EMP Burst": "tech",
    "Neural Hack": "tech",
    "Overdrive": "assault",
    "Nanite Swarm": "resilience",
}

_IMPLANT_SYNERGY = {
    "Cyberdeck": "tech",
    "Reflex Core": "stealth",
    "Optic Cortex": "assault",
}

_STAGES = (
    StageDefinition(
        name="Infiltration",
        challenge="Ghost through the perimeter sensors",
        focus_stat="stealth",
    ),
    StageDefinition(
        name="Combat Clash",
        challenge="Break the opposition strike team",
        focus_stat="assault",
    ),
    StageDefinition(
        name="Ghost Capture",
        challenge="Stabilize the target data ghost",
        focus_stat="tech",
    ),
    StageDefinition(
        name="Extraction",
        challenge="Escape the hot zone before reinforcements arrive",
        focus_stat="resilience",
    ),
)


def _name_lookup(
    items: Iterable[T],
    requested: str | None,
    *,
    item_type: str,
    rng: random.Random,
) -> T:
    """Find an item by name, or choose randomly if name is None."""
    sequence = list(items)
    if requested is None:
        return rng.choice(sequence)
    for item in sequence:
        if getattr(item, "name").lower() == requested.lower():
            return item
    raise ValueError(f"Unknown {item_type}: {requested}")


class SimulationEngine:
    """Run deterministic-feeling text simulations of Neon Ascendant missions."""

    def __init__(self, *, seed: int | None = None) -> None:
        self._random = random.Random(seed)
        self._codex = build_codex()
        self._generator = MissionGenerator(seed=seed)

    def _select_archetype(self, name: str | None) -> Archetype:
        return _name_lookup(
            self._codex["archetypes"], name, item_type="archetype", rng=self._random
        )

    def _select_district(self, name: str | None) -> District:
        return _name_lookup(
            self._codex["districts"], name, item_type="district", rng=self._random
        )

    def _build_brief(
        self,
        archetype_name: str | None,
        district_name: str | None,
    ) -> MissionBrief:
        """Build a mission brief, optionally constraining archetype and district."""
        if archetype_name is None and district_name is None:
            # Use the generator directly when no constraints
            return self._generator.craft_brief()
        
        # Build custom brief with constraints
        archetype = self._select_archetype(archetype_name)
        district = self._select_district(district_name)
        opposition = self._random.choice(self._codex["factions"])
        weapon = self._random.choice(self._codex["weapons"])
        implant = self._random.choice(self._codex["implants"])
        ability = self._random.choice(archetype.abilities)
        complication = self._random.choice(COMPLICATIONS)
        extraction = self._random.choice(EXTRACTION_CONDITIONS)
        return MissionBrief(
            district=district,
            opposition=opposition,
            archetype=archetype,
            primary_weapon=weapon,
            backup_implant=implant,
            featured_ability=ability,
            complication=complication,
            extraction_condition=extraction,
        )

    def simulate(
        self,
        *,
        archetype: str | None = None,
        district: str | None = None,
        difficulty: int = 6,
    ) -> MissionReport:
        if difficulty < 1:
            raise ValueError("difficulty must be positive")

        brief = self._build_brief(archetype, district)
        profile = _ARCHETYPE_PROFILES.get(brief.archetype.name)
        if profile is None:
            raise ValueError(f"No profile defined for archetype {brief.archetype.name}")

        stages: List[StageResult] = []
        all_success = True
        ability_focus = _ABILITY_SYNERGY.get(brief.featured_ability.name)
        implant_focus = _IMPLANT_SYNERGY.get(brief.backup_implant.slot)

        for index, stage in enumerate(_STAGES):
            stat_value = profile[stage.focus_stat]
            roll = stat_value + self._random.randint(DICE_ROLL_MIN, DICE_ROLL_MAX)
            threshold = difficulty + index + self._random.randint(
                DIFFICULTY_VARIANCE_MIN, DIFFICULTY_VARIANCE_MAX
            )
            narration_bits = [
                f"{stage.name}: {brief.district.hazards[index % len(brief.district.hazards)]} complicates the approach."
            ]

            if ability_focus == stage.focus_stat:
                roll += ABILITY_SYNERGY_BONUS
                narration_bits.append(
                    f"{brief.featured_ability.name} grants an edge during {stage.name.lower()}"
                )

            if implant_focus == stage.focus_stat:
                roll += IMPLANT_SYNERGY_BONUS
                narration_bits.append(
                    f"{brief.backup_implant.name} reinforces {stage.focus_stat} protocols"
                )

            success = roll >= threshold
            if success:
                narration_bits.append(
                    "The team holds momentum and presses forward."
                )
            else:
                narration_bits.append(
                    "Setbacks slow the push, burning through reserves."
                )
                all_success = False

            stages.append(
                StageResult(
                    name=stage.name,
                    challenge=stage.challenge,
                    roll=roll,
                    threshold=threshold,
                    success=success,
                    narration=" ".join(narration_bits),
                )
            )

        rewards: Sequence[str]
        if all_success:
            loot_weapon = self._random.choice(self._codex["weapons"])
            loot_implant = self._random.choice(self._codex["implants"])
            rewards = (
                f"Recovered schematic: {loot_weapon.name}",
                f"Prototype implant: {loot_implant.name}",
            )
        else:
            rewards = (
                "Partial data cache salvaged", "Requisition cost increases next drop"
            )

        return MissionReport(brief=brief, stages=tuple(stages), success=all_success, rewards=rewards)


def format_report(report: MissionReport) -> str:
    """Render a mission report as Markdown for quick sharing."""

    lines = [
        f"## Mission Outcome: {report.brief.district.name}",
        f"**Archetype:** {report.brief.archetype.name}",
        f"**Opposition:** {report.brief.opposition.name}",
        f"**Primary Weapon:** {report.brief.primary_weapon.name}",
        f"**Featured Ability:** {report.brief.featured_ability.name}",
        f"**Extraction Condition:** {report.brief.extraction_condition}",
        "",
    ]

    for stage in report.stages:
        status = "Success" if stage.success else "Setback"
        lines.extend(
            [
                f"### {stage.name} — {status}",
                f"*Challenge:* {stage.challenge}",
                f"*Roll:* {stage.roll} vs difficulty {stage.threshold}",
                stage.narration,
                "",
            ]
        )

    resolution = "Extraction successful" if report.success else "Extraction compromised"
    lines.append(f"**Result:** {resolution}.")
    lines.append("**Rewards:**")
    lines.extend(f"- {reward}" for reward in report.rewards)
    lines.append("")
    return "\n".join(lines)
