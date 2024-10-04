// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GP4Testing : ModuleRules
{
	public GP4Testing(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem", "GameplayTasks", "Niagara", "MovieScene", "LevelSequence" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });
    }
}
