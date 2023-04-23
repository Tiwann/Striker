// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Striker : ModuleRules
{
	public Striker(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",  });

		PrivateDependencyModuleNames.AddRange(new string[] { "EnhancedInput" });
	}
}
