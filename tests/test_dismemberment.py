"""Unit tests for the stylized dismemberment physics system."""

from __future__ import annotations

import random

from neon_ascendant.dismemberment import DismembermentSystem


def test_high_force_severs_limb() -> None:
    system = DismembermentSystem(rng=random.Random(2))

    event = system.resolve_impact(
        weapon_category="Railgun",
        attack_vector="frontal",
        target_resilience=2,
        armor_rating=0,
        gore_bias=0.8,
        bonus_force=4,
    )

    assert event.severity == "severed"
    assert "frontal blitz" in event.description.lower()
    assert "impulse" not in event.description  # ensure narration remains cinematic


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
    assert "cauterized" in event.description.lower() or "glancing" in event.description.lower()
