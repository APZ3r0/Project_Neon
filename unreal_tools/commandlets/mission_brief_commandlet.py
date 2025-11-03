"""Commandlet to generate mission brief assets directly within the Unreal Editor.

This script depends on the Unreal Python API and should be executed via the
`ue4editor-cmd` command or from the in-editor Python console.  Designers can
invoke the commandlet using the following CLI:

    ue4editor-cmd Project_Neon.uproject -run=MissionBriefCommandlet \
        -input=/Game/Missions/Data \
        -output=/Game/Missions/Briefs

The commandlet scans data assets inside *input* and emits rich text descriptions
using the mission brief schema used by the Python prototype (`src/neon_ascendant`).
"""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from typing import Iterable, List

import unreal


@dataclass
class MissionBrief:
    """Lightweight container that mirrors the Python prototype DTO."""

    mission_id: str
    title: str
    summary: str
    objectives: List[str]

    def to_text(self) -> str:
        bullet_list = "\n".join(f"- {objective}" for objective in self.objectives)
        return f"{self.title}\n\n{self.summary}\n\nObjectives:\n{bullet_list}"


class MissionBriefCommandlet(unreal.AutomatedCommandlet):
    """Generates localized mission brief assets from data blueprints."""

    def __init__(self):
        super().__init__()
        self.log_info("MissionBriefCommandlet initialized")

    @staticmethod
    def _gather_args() -> argparse.Namespace:
        parser = argparse.ArgumentParser(add_help=False)
        parser.add_argument("-input", dest="input_path", default="/Game/Missions/Data")
        parser.add_argument("-output", dest="output_path", default="/Game/Missions/Briefs")
        parser.add_argument("-locale", dest="locale", default="en")
        parser.add_argument("-overwrite", dest="overwrite", action="store_true")
        args, _ = parser.parse_known_args()
        return args

    def _load_data_assets(self, path: str) -> Iterable[unreal.Object]:
        asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
        assets = asset_registry.get_assets_by_path(path, recursive=True)
        for asset_data in assets:
            asset = asset_data.get_asset()
            if hasattr(asset, "mission_id"):
                yield asset

    def build_brief(self, asset: unreal.Object) -> MissionBrief:
        objectives = list(getattr(asset, "objectives", []))
        return MissionBrief(
            mission_id=str(getattr(asset, "mission_id", "")),
            title=str(getattr(asset, "title", "Untitled Mission")),
            summary=str(getattr(asset, "summary", "")),
            objectives=[str(obj) for obj in objectives],
        )

    def _save_text_asset(self, brief: MissionBrief, output_dir: str, locale: str, overwrite: bool) -> None:
        asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
        factory = unreal.TextAssetFactory()
        filename = f"{brief.mission_id}_{locale}" if brief.mission_id else f"Mission_{locale}"
        package_path = f"{output_dir}/{filename}"

        if not overwrite and unreal.EditorAssetLibrary.does_asset_exist(package_path):
            self.log_warning(f"Skipping existing brief: {package_path}")
            return

        text_asset = asset_tools.create_asset(
            asset_name=filename,
            package_path=output_dir,
            asset_class=unreal.TextAsset,
            factory=factory,
        )
        text_asset.set_editor_property("text", brief.to_text())
        unreal.EditorAssetLibrary.save_loaded_asset(text_asset)
        self.log_info(f"Generated mission brief asset at {package_path}")

    def generate(self, input_path: str, output_path: str, locale: str, overwrite: bool) -> None:
        self.log_info(
            f"Generating mission briefs from {input_path} into {output_path} (locale={locale})"
        )
        for asset in self._load_data_assets(input_path):
            brief = self.build_brief(asset)
            self._save_text_asset(brief, output_path, locale, overwrite)

    def main(self, *arguments):
        args = self._gather_args()
        self.generate(args.input_path, args.output_path, args.locale, args.overwrite)
        return 0


# Required by UE4/UE5 reflection system to register the commandlet class.
unreal.register_slate_post_tick_callback(lambda delta_time: None)


def generate_mission_briefs(
    input_path: str = "/Game/Missions/Data",
    output_path: str = "/Game/Missions/Briefs",
    locale: str = "en",
    overwrite: bool = False,
) -> None:
    """Convenience function for Python callers (e.g., Editor Utility Widgets)."""

    MissionBriefCommandlet().generate(input_path, output_path, locale, overwrite)
