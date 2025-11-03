"""Stylized dismemberment physics approximations for mission simulations."""

from __future__ import annotations

import random
from dataclasses import dataclass
from typing import Mapping, Sequence


@dataclass(frozen=True)
class LimbProfile:
    """Describe the physical tolerances of a limb."""

    name: str
    structure: int
    sever_window: int


@dataclass(frozen=True)
class DismemberEvent:
    """Outcome of a single impact resolved by the dismemberment system."""

    limb: str
    severity: str
    impulse: int
    resistance: int
    margin: int
    description: str

    def summary(self) -> str:
        """Return a condensed human-readable summary."""

        state = self.severity.capitalize()
        return f"{state} {self.limb.lower()} (impulse {self.impulse} vs resistance {self.resistance})"


_DEFAULT_LIMBS: Sequence[LimbProfile] = (
    LimbProfile(name="Head", structure=8, sever_window=6),
    LimbProfile(name="Torso", structure=12, sever_window=8),
    LimbProfile(name="Left Arm", structure=7, sever_window=5),
    LimbProfile(name="Right Arm", structure=7, sever_window=5),
    LimbProfile(name="Left Leg", structure=9, sever_window=6),
    LimbProfile(name="Right Leg", structure=9, sever_window=6),
)


_VECTOR_WEIGHTS: Mapping[str, Mapping[str, int]] = {
    "frontal": {"Head": 3, "Torso": 4, "Left Arm": 1, "Right Arm": 1, "Left Leg": 1, "Right Leg": 1},
    "flanking": {"Head": 1, "Torso": 2, "Left Arm": 3, "Right Arm": 3, "Left Leg": 1, "Right Leg": 1},
    "low": {"Head": 0, "Torso": 1, "Left Arm": 1, "Right Arm": 1, "Left Leg": 4, "Right Leg": 4},
    "aerial": {"Head": 3, "Torso": 3, "Left Arm": 1, "Right Arm": 1, "Left Leg": 1, "Right Leg": 1},
}


_WEAPON_FORCE: Mapping[str, tuple[int, int]] = {
    "SMG": (8, 2),
    "Railgun": (14, 6),
    "Experimental": (16, 8),
    "Melee": (12, 5),
}


class DismembermentSystem:
    """Resolve over-the-top limb damage for cinematic combat narration."""

    def __init__(
        self,
        *,
        rng: random.Random | None = None,
        limbs: Sequence[LimbProfile] | None = None,
    ) -> None:
        self._rng = rng or random.Random()
        self._limbs = tuple(limbs or _DEFAULT_LIMBS)

    def _select_limb(self, attack_vector: str) -> LimbProfile:
        weights = _VECTOR_WEIGHTS.get(attack_vector, {})
        if not weights:
            return self._rng.choice(self._limbs)

        population: list[LimbProfile] = []
        for limb in self._limbs:
            weight = weights.get(limb.name, 0)
            population.extend([limb] * max(weight, 1))
        return self._rng.choice(population)

    def _weapon_force(self, weapon_category: str) -> tuple[int, int]:
        return _WEAPON_FORCE.get(weapon_category, (10, 3))

    def resolve_impact(
        self,
        *,
        weapon_category: str,
        attack_vector: str,
        target_resilience: int,
        armor_rating: int = 0,
        gore_bias: float = 0.5,
        bonus_force: int = 0,
    ) -> DismemberEvent:
        """Resolve an impact and return a cinematic result."""

        limb = self._select_limb(attack_vector)
        base_force, variance = self._weapon_force(weapon_category)
        impulse = base_force + bonus_force + self._rng.randint(0, variance)
        resistance = limb.structure + armor_rating + target_resilience
        margin = impulse - resistance

        if margin >= limb.sever_window:
            severity = "severed"
        elif margin >= 0:
            severity = "mangled"
        else:
            severity = "intact"

        description = self._describe_event(
            limb=limb.name,
            severity=severity,
            attack_vector=attack_vector,
            gore_bias=gore_bias,
            margin=margin,
        )

        return DismemberEvent(
            limb=limb.name,
            severity=severity,
            impulse=impulse,
            resistance=resistance,
            margin=margin,
            description=description,
        )

    def _describe_event(
        self,
        *,
        limb: str,
        severity: str,
        attack_vector: str,
        gore_bias: float,
        margin: int,
    ) -> str:
        intensity = "muted" if gore_bias < 0.4 else "visceral" if gore_bias > 0.75 else "stylized"
        vector_text = {
            "frontal": "frontal blitz",
            "flanking": "flanking rush",
            "low": "low sweep",
            "aerial": "descending strike",
        }.get(attack_vector, "wild exchange")

        limb_lower = limb.lower()
        if severity == "severed":
            if intensity == "muted":
                gore_line = f"Clean severance at the {limb_lower}, leaving cauterized synth-flesh."
            elif intensity == "visceral":
                gore_line = f"{limb} spins free in an arc of neon plasma and arterial spray."
            else:
                gore_line = f"{limb} shears off with a crackle of overcharged monofilament."
        elif severity == "mangled":
            if intensity == "muted":
                gore_line = f"Armor buckles, leaving the {limb_lower} hanging at odd angles."
            elif intensity == "visceral":
                gore_line = (
                    f"Polyfiber muscle ruptures and exposes sparking implants along the {limb_lower}."
                )
            else:
                gore_line = f"Microfractures ripple through the {limb_lower}, disabling it."
        else:
            if intensity == "muted":
                gore_line = f"Glancing blow—{limb_lower} remains intact."
            elif intensity == "visceral":
                gore_line = f"Impact bruises the {limb_lower}, splashing synth-blood but no separation."
            else:
                gore_line = f"Deflective sparks dance across the {limb_lower} as the strike is absorbed."

        momentum_line = "Momentum surges" if margin > 0 else "Target reels but stabilizes"
        return f"{vector_text.title()} delivers {gore_line} {momentum_line}."


__all__ = ["DismembermentSystem", "DismemberEvent", "LimbProfile"]
