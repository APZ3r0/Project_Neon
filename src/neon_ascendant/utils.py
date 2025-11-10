"""Shared utility functions for the Neon Ascendant package."""

from __future__ import annotations

from pathlib import Path
from typing import TypeVar

T = TypeVar("T")


def repository_root() -> Path:
    """Return the absolute path to the repository root.
    
    The module lives in ``src/neon_ascendant`` so two ``parent`` calls walk back to
    the project directory regardless of the import location.
    """
    return Path(__file__).resolve().parents[2]

