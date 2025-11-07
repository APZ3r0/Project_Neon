"""Approximate dismemberment physics tuned for text-based mission simulations."""

from __future__ import annotations

import random
from dataclasses import dataclass
from typing import Mapping, Sequence


@dataclass(frozen=True)
class LimbProfile:
    """Describe the structural tolerances of a limb segment."""

    name: str
    bone_strength: float
    shear_threshold: float
    mass: float
    surface_area: float


@dataclass(frozen=True)
class WeaponProfile:
    """Physical properties for a weapon category."""

    name: str
    projectile_mass: float
    velocity: float
    sharpness: float
    impulse_variance: float


@dataclass(frozen=True)
class DismemberEvent:
    """Outcome of a single impact resolved by the dismemberment system."""

    limb: str
    severity: str
    impulse: float
    resistance: float
    margin: float
    description: str

    def summary(self) -> str:
        """Return a condensed human-readable summary."""

        state = self.severity.capitalize()
        impulse = round(self.impulse, 1)
        resistance = round(self.resistance, 1)
        return f"{state} {self.limb.lower()} (impulse {impulse} vs resistance {resistance})"


_DEFAULT_LIMBS: Sequence[LimbProfile] = (
    LimbProfile(name="Head", bone_strength=5.0, shear_threshold=3.2, mass=4.5, surface_area=0.03),
    LimbProfile(name="Torso", bone_strength=8.5, shear_threshold=5.0, mass=32.0, surface_area=0.12),
    LimbProfile(name="Left Arm", bone_strength=4.0, shear_threshold=2.8, mass=6.0, surface_area=0.04),
    LimbProfile(name="Right Arm", bone_strength=4.0, shear_threshold=2.8, mass=6.0, surface_area=0.04),
    LimbProfile(name="Left Leg", bone_strength=5.5, shear_threshold=3.4, mass=11.0, surface_area=0.06),
    LimbProfile(name="Right Leg", bone_strength=5.5, shear_threshold=3.4, mass=11.0, surface_area=0.06),
)


_VECTOR_WEIGHTS: Mapping[str, Mapping[str, int]] = {
    "frontal": {"Head": 3, "Torso": 4, "Left Arm": 1, "Right Arm": 1, "Left Leg": 1, "Right Leg": 1},
    "flanking": {"Head": 1, "Torso": 2, "Left Arm": 3, "Right Arm": 3, "Left Leg": 1, "Right Leg": 1},
    "low": {"Head": 0, "Torso": 1, "Left Arm": 1, "Right Arm": 1, "Left Leg": 4, "Right Leg": 4},
    "aerial": {"Head": 3, "Torso": 3, "Left Arm": 1, "Right Arm": 1, "Left Leg": 1, "Right Leg": 1},
}


_VECTOR_MODIFIERS: Mapping[str, float] = {
    "frontal": 1.0,
    "flanking": 0.9,
    "low": 0.85,
    "aerial": 1.1,
}


_WEAPON_LIBRARY: Mapping[str, WeaponProfile] = {
    "SMG": WeaponProfile(name="SMG", projectile_mass=0.007, velocity=610.0, sharpness=0.5, impulse_variance=12.0),
    "Railgun": WeaponProfile(
        name="Railgun", projectile_mass=0.02, velocity=1500.0, sharpness=0.8, impulse_variance=25.0
    ),
    "Experimental": WeaponProfile(
        name="Experimental", projectile_mass=0.05, velocity=900.0, sharpness=1.2, impulse_variance=30.0
    ),
    "Melee": WeaponProfile(name="Melee", projectile_mass=2.8, velocity=35.0, sharpness=0.7, impulse_variance=18.0),
}


class DismembermentSystem:
    """Resolve limb damage using lightweight physics approximations."""

    def __init__(
        self,
        *,
        rng: random.Random | None = None,
        limbs: Sequence[LimbProfile] | None = None,
        weapon_library: Mapping[str, WeaponProfile] | None = None,
    ) -> None:
        self._rng = rng or random.Random()
        self._limbs = tuple(limbs or _DEFAULT_LIMBS)
        self._weapon_library = weapon_library or _WEAPON_LIBRARY

    def _select_limb(self, attack_vector: str) -> LimbProfile:
        weights = _VECTOR_WEIGHTS.get(attack_vector, {})
        if not weights:
            return self._rng.choice(self._limbs)

        population: list[LimbProfile] = []
        for limb in self._limbs:
            weight = weights.get(limb.name, 0)
            population.extend([limb] * max(weight, 1))
        return self._rng.choice(population)

    def _weapon_profile(self, weapon_category: str) -> WeaponProfile:
        return self._weapon_library.get(weapon_category, _WEAPON_LIBRARY["Melee"])

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
        weapon = self._weapon_profile(weapon_category)

        vector_modifier = _VECTOR_MODIFIERS.get(attack_vector, 1.0)
        # Treat ``bonus_force`` as additional metres per second contributed by abilities.
        velocity = max(0.0, weapon.velocity + float(bonus_force) * 25.0)
        impulse_base = weapon.projectile_mass * velocity
        impulse_variation = self._rng.uniform(-weapon.impulse_variance, weapon.impulse_variance)
        impulse = max(0.0, (impulse_base + impulse_variation) * vector_modifier)

        # Approximate peak force from impulse over a small contact window. Fast strikes
        # experience shorter contact times than slower melee hits.
        base_contact = 0.003 if velocity > 500 else 0.006
        contact_time = base_contact + limb.mass * 0.00005
        force = impulse / max(contact_time, 1e-4)

        # Apply damping from resilience and armour. The constants are tuned to keep the
        # model numerically stable while still reflecting stronger targets.
        damping_multiplier = 1.0 + 0.08 * max(target_resilience, 0)
        armour_absorption = 1.0 - min(0.75, 0.07 * max(armor_rating, 0))
        effective_force = force * armour_absorption

        surface_factor = 1.0 + limb.surface_area * 8.0
        resistance_force = (limb.bone_strength * 1000.0) * damping_multiplier * surface_factor
        sever_force = (limb.shear_threshold * 1000.0) * damping_multiplier * surface_factor
        margin_force = effective_force - resistance_force

        severity: str
        if margin_force >= sever_force:
            severity = "severed"
        elif margin_force >= -0.2 * resistance_force:
            severity = "mangled"
        else:
            severity = "intact"

        description = self._describe_event(
            limb=limb.name,
            severity=severity,
            attack_vector=attack_vector,
            gore_bias=gore_bias,
            margin=margin_force / 1000.0,
            impulse=impulse,
            resistance=resistance_force,
        )

        return DismemberEvent(
            limb=limb.name,
            severity=severity,
            impulse=impulse,
            resistance=resistance_force / 1000.0,
            margin=margin_force / 1000.0,
            description=description,
        )

    def _describe_event(
        self,
        *,
        limb: str,
        severity: str,
        attack_vector: str,
        gore_bias: float,
        margin: float,
        impulse: float,
        resistance: float,
    ) -> str:
        intensity = "sterile" if gore_bias < 0.35 else "graphic" if gore_bias > 0.75 else "cinematic"
        vector_text = {
            "frontal": "frontal charge",
            "flanking": "flanking sweep",
            "low": "low sweep",
            "aerial": "descending strike",
        }.get(attack_vector, "chaotic exchange")

        limb_lower = limb.lower()
        impulse_text = f"{round(impulse, 1)} N·s"
        resistance_text = f"{round(resistance / 1000.0, 1)} kN"

        if severity == "severed":
            if intensity == "sterile":
                outcome = f"Clean separation at the {limb_lower}; actuators shear without spatter."
            elif intensity == "graphic":
                outcome = f"{limb} detaches amid sparking implants and coolant spray."
            else:
                outcome = f"{limb} severs as reinforced tendons snap in a pulse of ionized air."
        elif severity == "mangled":
            if intensity == "sterile":
                outcome = f"Impact crushes structural supports, leaving the {limb_lower} limp."
            elif intensity == "graphic":
                outcome = f"The {limb_lower} twists, plating torn back with exposed conduits."
            else:
                outcome = f"Torsion fractures ripple through the {limb_lower}, disabling it."
        else:
            if intensity == "sterile":
                outcome = f"Force disperses across armour; the {limb_lower} stays operational."
            elif intensity == "graphic":
                outcome = f"The strike bruises synth-flesh but fails to breach core plating."
            else:
                outcome = f"Deflection fields flare, absorbing the blow around the {limb_lower}."

        momentum_line = "Momentum surges forward" if margin > 0 else "Target regains stance"
        return f"{vector_text.title()} hits with {impulse_text} against {resistance_text}. {outcome} {momentum_line}."


__all__ = ["DismembermentSystem", "DismemberEvent", "LimbProfile", "WeaponProfile"]
