import json
import re
from pathlib import Path

import pytest

PROJECT_ROOT = Path(__file__).resolve().parents[1]
SOURCE_DIR = PROJECT_ROOT / "Source" / "NeonAscendant"


@pytest.fixture(scope="module")
def mission_files():
    header = (SOURCE_DIR / "Public" / "MissionGenerator.h").read_text(encoding="utf-8")
    data_cpp = (SOURCE_DIR / "Private" / "MissionData.cpp").read_text(encoding="utf-8")
    return header, data_cpp


def test_uproject_declares_runtime_module():
    contents = json.loads((PROJECT_ROOT / "NeonAscendant.uproject").read_text(encoding="utf-8"))
    assert contents["Modules"], "The Unreal project must declare at least one module."
    module = contents["Modules"][0]
    assert module["Name"] == "NeonAscendant"
    assert module["Type"] == "Runtime"


def test_build_configuration_links_core_dependencies():
    build_file = (SOURCE_DIR / "NeonAscendant.Build.cs").read_text(encoding="utf-8")
    for dependency in ("Core", "CoreUObject", "Engine", "GameplayTags"):
        assert dependency in build_file


def test_mission_generator_exposes_blueprint_api(mission_files):
    header, _ = mission_files
    assert "UCLASS(BlueprintType)" in header
    assert "FMissionBrief GenerateMissionBrief" in header
    assert "GenerateMissionBriefs" in header
    assert "SeedGenerator" in header
    assert "UCLASS(Config=Game)" in header
    assert "static UMissionGenerator* GetGenerator" in header


def test_data_sets_include_all_original_concepts(mission_files):
    _, data_cpp = mission_files
    expected_strings = [
        "Neon Abyss",
        "Spire District",
        "Ghost Grid",
        "Helix Corp",
        "Vanta Syndicate",
        "Dawnbreakers",
        "Pulsecaster SMG",
        "Helix Rail Rifle",
        "Singularity Projector",
        "Cyberdeck Mk.IV",
        "Reflex Core X",
        "Optic Cortex Prism",
    ]
    for item in expected_strings:
        assert item in data_cpp


def test_complications_and_extraction_conditions_defined(mission_files):
    _, data_cpp = mission_files
    complication_matches = re.findall(r"Complications\s*=\s*\{([^}]*)\}", data_cpp)
    extraction_matches = re.findall(r"ExtractionConditions\s*=\s*\{([^}]*)\}", data_cpp)
    assert complication_matches, "Complication table must be defined in MissionData.cpp"
    assert extraction_matches, "Extraction condition table must be defined in MissionData.cpp"
    assert complication_matches[0].count("TEXT(") >= 4
    assert extraction_matches[0].count("TEXT(") >= 4
