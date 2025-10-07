"""Unit tests for the Neon Ascendant mission generator."""

from neon_ascendant.game import MissionGenerator


def test_craft_briefs_respects_count(tmp_path, monkeypatch=None):
    generator = MissionGenerator(seed=42)
    briefs = generator.craft_briefs(2)
    assert len(briefs) == 2


def test_craft_briefs_positive():
    generator = MissionGenerator(seed=1)
    try:
        generator.craft_briefs(0)
    except ValueError:
        assert True
    else:
        raise AssertionError("Expected ValueError when count <= 0")


def test_markdown_output(tmp_path):
    generator = MissionGenerator(seed=7)
    brief = generator.craft_brief()
    output = brief.to_markdown()
    assert "Operation" in output
    assert brief.district.name in output
    assert brief.opposition.name in output
