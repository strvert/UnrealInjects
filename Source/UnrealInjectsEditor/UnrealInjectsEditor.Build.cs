using UnrealBuildTool;

public class UnrealInjectsEditor : ModuleRules
{
	public UnrealInjectsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"BlueprintGraph",
				"ToolMenus",
				"WorkspaceMenuStructure",
				"UnrealEd",
				"UnrealInjects",
				"AssetTools"
			}
		);
	}
}