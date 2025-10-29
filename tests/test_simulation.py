"""Tests for the Neon Ascendant mission simulation module."""

from __future__ import annotations

from neon_ascendant.simulation import SimulationEngine, format_report


def test_simulation_deterministic_seed() -> None:
    engine = SimulationEngine(seed=7)
    report = engine.simulate(archetype="Specter", district="Ghost Grid", difficulty=7)

    assert report.brief.archetype.name == "Specter"
    assert report.brief.district.name == "Ghost Grid"
    assert len(report.stages) == 4
    assert [stage.success for stage in report.stages] == [True, True, True, False]
    assert report.success is False
    assert "Partial data cache" in report.rewards[0]


def test_format_report_includes_stage_sections() -> None:
    engine = SimulationEngine(seed=3)
    report = engine.simulate()
    rendered = format_report(report)

    assert report.brief.archetype.name in rendered
    for stage in report.stages:
        assert stage.name in rendered
        assert stage.challenge in rendered
