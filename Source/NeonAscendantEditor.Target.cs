using UnrealBuildTool;
using System.Collections.Generic;

public class NeonAscendantEditorTarget : TargetRules
{
	public NeonAscendantEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("NeonAscendant");
	}
}
