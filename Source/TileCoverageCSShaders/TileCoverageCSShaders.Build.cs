using UnrealBuildTool;

public class TileCoverageCSShaders : ModuleRules
{
    public TileCoverageCSShaders(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "RenderCore",
                "RHI"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
            }
        );
    }
}