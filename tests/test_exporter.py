from __future__ import annotations

import os
from pathlib import Path

import pytest

from neon_ascendant.exporter import export_repository


def test_export_repository_copies_expected_content(tmp_path: Path) -> None:
    destination = tmp_path / "export"
    export_repository(destination)

    # Ensure core directories and files are present in the exported structure.
    assert (destination / "src" / "neon_ascendant" / "data.py").is_file()
    assert (destination / "tests" / "test_game.py").is_file()
    assert (destination / "README.md").is_file()


def test_export_repository_rejects_nested_destination(tmp_path: Path) -> None:
    inside_repo = Path.cwd() / "subdir"
    inside_repo.mkdir(exist_ok=True)

    with pytest.raises(ValueError):
        export_repository(inside_repo)

    # Clean up the helper directory to avoid polluting the workspace.
    os.rmdir(inside_repo)
