from pathlib import Path
import zipfile

import pytest

from neon_ascendant.archive import create_archive


def test_create_archive_produces_bundle_with_key_assets(tmp_path: Path) -> None:
    output = tmp_path / "neon_bundle.zip"
    archive_path = create_archive(output)

    assert archive_path == output
    assert archive_path.is_file()

    with zipfile.ZipFile(archive_path) as bundle:
        contents = set(bundle.namelist())

    assert "README.md" in contents
    assert "src/neon_ascendant/data.py" in contents
    assert "tests/test_simulation.py" in contents


def test_create_archive_can_skip_tests(tmp_path: Path) -> None:
    output = tmp_path / "no_tests.zip"
    create_archive(output, include_tests=False)

    with zipfile.ZipFile(output) as bundle:
        assert all(not name.startswith("tests/") for name in bundle.namelist())


def test_create_archive_rejects_directory_destination(tmp_path: Path) -> None:
    target_dir = tmp_path / "output_dir"
    target_dir.mkdir()

    with pytest.raises(ValueError):
        create_archive(target_dir)
