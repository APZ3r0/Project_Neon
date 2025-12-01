using UnrealBuildTool;
using System.Collections.Generic;

public class NeonAscendantTarget : TargetRules
{
	public NeonAscendantTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("NeonAscendant");
	}
}
