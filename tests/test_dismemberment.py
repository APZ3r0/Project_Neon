"""Unit tests for the stylized dismemberment physics system."""

from __future__ import annotations

import random

import pytest

from neon_ascendant.dismemberment import (
    DismembermentSystem,
    LimbProfile,
    WeaponProfile,
)


def test_high_force_severs_limb() -> None:
    system = DismembermentSystem(rng=random.Random(1))

    event = system.resolve_impact(
        weapon_category="Railgun",
        attack_vector="frontal",
        target_resilience=1,
        armor_rating=0,
        gore_bias=0.8,
        bonus_force=8,
    )

    assert event.severity == "severed"
    assert event.margin > 0
    assert "frontal charge" in event.description.lower()
    assert "n·s" in event.description.lower()


def test_muted_gore_bias_softens_description() -> None:
    system = DismembermentSystem(rng=random.Random(5))

    event = system.resolve_impact(
        weapon_category="SMG",
        attack_vector="flanking",
        target_resilience=6,
        armor_rating=2,
        gore_bias=0.2,
    )

    assert event.severity in {"intact", "mangled"}
    assert "force disperses" in event.description.lower() or "clean separation" in event.description.lower()


def test_custom_weapon_library_allows_overrides() -> None:
    weapon = WeaponProfile(name="Test", projectile_mass=1.0, velocity=80.0, sharpness=1.0, impulse_variance=0.0)
    limb = LimbProfile(name="Test Limb", bone_strength=1.2, shear_threshold=0.8, mass=4.0, surface_area=0.02)
    system = DismembermentSystem(
        rng=random.Random(7),
        limbs=(limb,),
        weapon_library={"Test": weapon},
    )

    event = system.resolve_impact(
        weapon_category="Test",
        attack_vector="frontal",
        target_resilience=0,
        armor_rating=0,
        gore_bias=0.5,
    )

    assert event.severity == "severed"
    assert event.impulse == pytest.approx(80.0, rel=1e-6)
