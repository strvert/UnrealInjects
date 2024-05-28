using UnrealBuildTool;

public class DpendencyInjectionResolver : ModuleRules
{
    public DpendencyInjectionResolver(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "DependencyInjection"
            }
        );
    }
}