"""CLI entry point for running Neon Ascendant mission simulations."""

from __future__ import annotations

import argparse
from pathlib import Path

from .simulation import SimulationEngine, format_report


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--archetype",
        type=str,
        default=None,
        help="force a specific archetype (defaults to random)",
    )
    parser.add_argument(
        "--district",
        type=str,
        default=None,
        help="force a specific district (defaults to random)",
    )
    parser.add_argument(
        "--difficulty",
        type=int,
        default=6,
        help="baseline difficulty modifier (1+)",
    )
    parser.add_argument(
        "--seed",
        type=int,
        default=None,
        help="optional RNG seed for reproducible runs",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=None,
        help="optional markdown file destination",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    engine = SimulationEngine(seed=args.seed)
    report = engine.simulate(
        archetype=args.archetype,
        district=args.district,
        difficulty=args.difficulty,
    )
    rendered = format_report(report)
    if args.output:
        args.output.write_text(rendered, encoding="utf-8")
    else:
        print(rendered)


if __name__ == "__main__":  # pragma: no cover - CLI entry point
    main()
