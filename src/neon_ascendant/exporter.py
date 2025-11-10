"""Utilities for exporting the Neon Ascendant prototype to another location."""

from __future__ import annotations

import argparse
import shutil
from pathlib import Path
from typing import Iterable

from .utils import repository_root

REPO_IGNORES: tuple[str, ...] = (
    ".git",
    ".pytest_cache",
    ".mypy_cache",
    ".venv",
    "__pycache__",
)

FILE_IGNORES: tuple[str, ...] = (".gitignore",)


def export_repository(destination: Path | str, include_tests: bool = True) -> Path:
    """Copy the repository contents into ``destination``.

    Parameters
    ----------
    destination:
        Target directory for the exported files. The directory must either not
        exist or be empty.
    include_tests:
        When ``True`` (default), the ``tests`` package is copied alongside the
        source code so the exported workspace can run the project's unit tests.

    Returns
    -------
    Path
        The resolved path to ``destination``.

    Raises
    ------
    ValueError
        If the destination directory is inside the repository root or already
        contains files.
    """

    target = Path(destination).expanduser().resolve()
    root = repository_root()

    if target == root or target.is_relative_to(root):
        raise ValueError("Destination must live outside of the repository root.")

    if target.exists():
        # avoid partially copying over a populated directory
        if any(target.iterdir()):
            raise ValueError("Destination directory must be empty before export.")
    else:
        target.mkdir(parents=True, exist_ok=True)

    def _should_skip(entry: Path) -> bool:
        if entry.name in REPO_IGNORES:
            return True
        if entry.is_file() and entry.name in FILE_IGNORES:
            return True
        if entry.name == "tests" and not include_tests:
            return True
        return False

    for entry in root.iterdir():
        if _should_skip(entry):
            continue

        destination_path = target / entry.name
        if entry.is_dir():
            shutil.copytree(entry, destination_path, ignore=shutil.ignore_patterns("__pycache__", "*.pyc"))
        else:
            shutil.copy2(entry, destination_path)

    return target


def _parse_args(argv: Iterable[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Export the Neon Ascendant repository to a new location.")
    parser.add_argument(
        "destination",
        help=(
            "Path where the repository should be copied. Useful for staging the project in a separate "
            "workspace such as a GitHub Codespace."
        ),
    )
    parser.add_argument(
        "--no-tests",
        action="store_true",
        help="Skip copying the tests directory into the export target.",
    )
    return parser.parse_args(argv)


def main(argv: Iterable[str] | None = None) -> Path:
    """Entry point for ``python -m neon_ascendant.exporter``."""

    args = _parse_args(argv)
    return export_repository(args.destination, include_tests=not args.no_tests)


if __name__ == "__main__":  # pragma: no cover - exercised via CLI
    main()
