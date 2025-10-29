"""High level mission scaffolding for the Neon Ascendant prototype."""

from __future__ import annotations

import random
from dataclasses import dataclass
from typing import Iterable, Sequence

from .data import Ability, Archetype, District, Faction, Implant, Weapon, build_codex


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

    _complications: Sequence[str] = (
        "Ghost Grid instabilities cause random HUD distortion",
        "Helix orbital overwatch sweeps disrupt cloak cycles",
        "Dawnbreaker converts attempt to hack your cyberdeck mid-fight",
        "Rival Ascendant strike team is pursuing the same data ghost",
    )

    _extraction_conditions: Sequence[str] = (
        "Secure a clean uplink and survive the counter-hack timer",
        "Evacuate via hijacked mag-lev within 90 seconds of objective completion",
        "Carry data-core physically to an Ascendant drop pod",
        "Maintain over 50% armor integrity for premium rewards",
    )

    def __init__(self, *, seed: int | None = None) -> None:
        self._codex = build_codex()
        self._random = random.Random(seed)

    def _choose(self, options: Iterable) -> object:
        return self._random.choice(tuple(options))

    def craft_brief(self) -> MissionBrief:
        """Create a mission brief using weighted randomness."""

        district = self._choose(self._codex["districts"])
        opposition = self._choose(self._codex["factions"])
        archetype = self._choose(self._codex["archetypes"])
        primary_weapon = self._choose(self._codex["weapons"])
        backup_implant = self._choose(self._codex["implants"])
        featured_ability = self._choose(archetype.abilities)
        complication = self._choose(self._complications)
        extraction_condition = self._choose(self._extraction_conditions)

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
