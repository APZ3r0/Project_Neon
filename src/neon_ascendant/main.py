"""Simple CLI entrypoint for generating Neon Ascendant mission briefs."""

from __future__ import annotations

import argparse
from pathlib import Path
from typing import Iterable

from .game import MissionGenerator


def _write_output(briefs: Iterable[str], destination: Path | None) -> None:
    content = "\n".join(briefs)
    if destination:
        destination.write_text(content, encoding="utf-8")
    else:
        print(content)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--count",
        type=int,
        default=3,
        help="number of mission briefs to generate",
    )
    parser.add_argument(
        "--seed",
        type=int,
        default=None,
        help="optional RNG seed for reproducible briefs",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=None,
        help="optional path to write markdown output",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    generator = MissionGenerator(seed=args.seed)
    briefs = [brief.to_markdown() for brief in generator.craft_briefs(args.count)]
    _write_output(briefs, args.output)


if __name__ == "__main__":  # pragma: no cover - CLI entry point
    main()
