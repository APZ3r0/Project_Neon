"""High level mission scaffolding for the Neon Ascendant prototype."""

from __future__ import annotations

import random
from dataclasses import dataclass
from typing import Iterable, Sequence, TypeVar

T = TypeVar("T")

from .data import (
    COMPLICATIONS,
    EXTRACTION_CONDITIONS,
    Ability,
    Archetype,
    District,
    Faction,
    Implant,
    Weapon,
    build_codex,
)


@dataclass
class MissionBrief:
    """A generated mission pitch derived from the concept bible."""

    district: District
    opposition: Faction
    archetype: Archetype
    primary_weapon: Weapon
    backup_implant: Implant
    featured_ability: Ability
    complication: str
    extraction_condition: str

    def to_markdown(self) -> str:
        """Render the brief in a format that can drop into documentation."""

        lines = [
            f"### Operation: {self.district.name}",
            f"**Opposition:** {self.opposition.name} — {self.opposition.philosophy}",
            f"**Primary Archetype:** {self.archetype.name} ({self.archetype.role})",
            f"**Signature Ability:** {self.featured_ability.name} — {self.featured_ability.description}",
            f"**Loadout Anchor:** {self.primary_weapon.name} — {self.primary_weapon.damage_profile}",
            f"**Adaptive Implant:** {self.backup_implant.name} ({self.backup_implant.slot})",
            f"**Complication:** {self.complication}",
            f"**Extraction Condition:** {self.extraction_condition}",
            "",
        ]
        return "\n".join(lines)


class MissionGenerator:
    """Generate flavor-rich mission briefs to help visualize gameplay loops."""

    def __init__(self, *, seed: int | None = None) -> None:
        self._codex = build_codex()
        self._random = random.Random(seed)

    def _choose(self, options: Iterable[T]) -> T:
        """Randomly select an item from the given options."""
        return self._random.choice(tuple(options))

    def craft_brief(self) -> MissionBrief:
        """Create a mission brief using weighted randomness."""

        district = self._choose(self._codex["districts"])
        opposition = self._choose(self._codex["factions"])
        archetype = self._choose(self._codex["archetypes"])
        primary_weapon = self._choose(self._codex["weapons"])
        backup_implant = self._choose(self._codex["implants"])
        featured_ability = self._choose(archetype.abilities)
        complication = self._choose(COMPLICATIONS)
        extraction_condition = self._choose(EXTRACTION_CONDITIONS)

        return MissionBrief(
            district=district,
            opposition=opposition,
            archetype=archetype,
            primary_weapon=primary_weapon,
            backup_implant=backup_implant,
            featured_ability=featured_ability,
            complication=complication,
            extraction_condition=extraction_condition,
        )

    def craft_briefs(self, count: int = 3) -> Sequence[MissionBrief]:
        """Generate multiple mission briefs for rapid prototyping."""

        if count <= 0:
            raise ValueError("count must be positive")
        return [self.craft_brief() for _ in range(count)]


if __name__ == "__main__":
    gen = MissionGenerator()
    briefs = gen.craft_briefs()
    for brief in briefs:
        print(brief.to_markdown())
