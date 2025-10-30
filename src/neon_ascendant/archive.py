"""Archive helpers for packaging the Neon Ascendant prototype."""

from __future__ import annotations

import argparse
import subprocess
import zipfile
from pathlib import Path
from typing import Iterable, Sequence


def _repository_root() -> Path:
    """Return the absolute path to the repository root."""

    return Path(__file__).resolve().parents[2]


def _git_tracked_files(root: Path) -> Sequence[Path]:
    """Return the tracked files relative to ``root`` using ``git ls-files``."""

    result = subprocess.run(
        ["git", "ls-files"],
        check=True,
        capture_output=True,
        text=True,
        cwd=root,
    )
    files: list[Path] = []
    for line in result.stdout.splitlines():
        line = line.strip()
        if not line:
            continue
        files.append(root / line)
    return files


def create_archive(destination: Path | str, *, include_tests: bool = True) -> Path:
    """Create a ZIP archive of the repository at ``destination``.

    Parameters
    ----------
    destination:
        Target file path for the generated archive. Parent directories are
        created automatically when necessary.
    include_tests:
        When ``True`` (default), files under ``tests/`` are bundled alongside the
        source code so the archive can run the existing unit tests.

    Returns
    -------
    Path
        The resolved output path of the generated archive.
    """

    output = Path(destination).expanduser().resolve()
    if output.exists() and output.is_dir():
        raise ValueError("Destination must point to a file, not a directory.")

    output.parent.mkdir(parents=True, exist_ok=True)

    root = _repository_root()
    tracked_files = _git_tracked_files(root)

    with zipfile.ZipFile(output, "w", compression=zipfile.ZIP_DEFLATED) as archive:
        for file_path in tracked_files:
            relative = file_path.relative_to(root)
            if not include_tests and relative.parts and relative.parts[0] == "tests":
                continue
            archive.write(file_path, arcname=str(relative))

    return output


def _parse_args(argv: Iterable[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Create a ZIP bundle of the Neon Ascendant project.")
    parser.add_argument(
        "destination",
        help="Path where the archive should be written (e.g. dist/neon_ascendant_bundle.zip).",
    )
    parser.add_argument(
        "--no-tests",
        action="store_true",
        help="Skip packaging the tests directory into the archive.",
    )
    return parser.parse_args(argv)


def main(argv: Iterable[str] | None = None) -> Path:
    """Entry point for ``python -m neon_ascendant.archive``."""

    args = _parse_args(argv)
    return create_archive(args.destination, include_tests=not args.no_tests)


if __name__ == "__main__":  # pragma: no cover - exercised via CLI
    main()
