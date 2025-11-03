"""Blueprintable editor utility widget integration for mission brief generation.

When converted into a Blueprint (Editor Utility Widget), designers can trigger
mission brief generation from the Unreal Editor UI without Python knowledge.

Usage inside Unreal:

1. Create a new Editor Utility Widget blueprint that subclasses
   `MissionBriefGeneratorWidget`.
2. Add UI controls that call `run_generation` with the desired parameters.
3. Optionally bind `preview_brief_for_asset` to preview text before saving.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import List, Optional

import unreal

from ..commandlets.mission_brief_commandlet import MissionBrief, MissionBriefCommandlet


@dataclass
class MissionBriefPreview:
    """Preview payload exposed to Blueprint."""

    mission_id: str
    title: str
    summary: str
    objectives: List[str]
    preview_text: str


class MissionBriefGeneratorWidget(unreal.EditorUtilityWidget):
    """Provides Blueprint-callable methods to orchestrate mission brief creation."""

    input_path: str = "/Game/Missions/Data"
    output_path: str = "/Game/Missions/Briefs"
    locale: str = "en"
    overwrite: bool = False

    @unreal.ufunction(override=True)
    def on_initialized(self) -> None:
        unreal.log("MissionBriefGeneratorWidget initialized")

    @unreal.ufunction(blueprint_callable=True)
    def run_generation(
        self,
        input_path: str = "",
        output_path: str = "",
        locale: str = "",
        overwrite: Optional[bool] = None,
    ) -> None:
        commandlet = MissionBriefCommandlet()
        commandlet.generate(
            input_path or self.input_path,
            output_path or self.output_path,
            locale or self.locale,
            self.overwrite if overwrite is None else overwrite,
        )

    @unreal.ufunction(blueprint_callable=True)
    def preview_brief_for_asset(self, asset_path: str) -> MissionBriefPreview:
        asset = unreal.load_object(None, asset_path)
        if asset is None:
            raise ValueError(f"Unable to load asset: {asset_path}")

        commandlet = MissionBriefCommandlet()
        brief = commandlet.build_brief(asset)
        return MissionBriefPreview(
            mission_id=brief.mission_id,
            title=brief.title,
            summary=brief.summary,
            objectives=brief.objectives,
            preview_text=brief.to_text(),
        )
