"""Unit tests for the Neon Ascendant mission generator."""

import pytest

from neon_ascendant.game import MissionGenerator


def test_craft_briefs_respects_count() -> None:
    generator = MissionGenerator(seed=42)
    briefs = generator.craft_briefs(2)
    assert len(briefs) == 2


def test_craft_briefs_requires_positive_count() -> None:
    generator = MissionGenerator(seed=1)
    with pytest.raises(ValueError):
        generator.craft_briefs(0)


def test_markdown_output(tmp_path) -> None:
    generator = MissionGenerator(seed=7)
    brief = generator.craft_brief()
    output = brief.to_markdown()
    assert "Operation" in output
    assert brief.district.name in output
    assert brief.opposition.name in output
