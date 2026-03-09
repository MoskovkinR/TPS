// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPS : ModuleRules
{
	public TPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"TPS",
			"TPS/Variant_Platforming",
			"TPS/Variant_Platforming/Animation",
			"TPS/Variant_Combat",
			"TPS/Variant_Combat/AI",
			"TPS/Variant_Combat/Animation",
			"TPS/Variant_Combat/Gameplay",
			"TPS/Variant_Combat/Interfaces",
			"TPS/Variant_Combat/UI",
			"TPS/Variant_SideScrolling",
			"TPS/Variant_SideScrolling/AI",
			"TPS/Variant_SideScrolling/Gameplay",
			"TPS/Variant_SideScrolling/Interfaces",
			"TPS/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
