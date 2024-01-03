// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class ProjectCompany : ModuleRules
{
	public ProjectCompany(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HeadMountedDisplay", "EnhancedInput", "OnlineSubsystemEOS", "OnlineSubsystem", "OnlineSubsystemUtils", "NavigationSystem", "SlateCore", "LevelSequence", "MovieScene", "Networking", "Sockets" });
    }
}
