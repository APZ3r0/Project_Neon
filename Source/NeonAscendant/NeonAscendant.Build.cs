using UnrealBuildTool;

public class NeonAscendant : ModuleRules
{
    public NeonAscendant(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "GameplayTags"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Projects"
        });

        bEnableExceptions = true;
    }
}
