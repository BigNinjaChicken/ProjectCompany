// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SteamCompany : ModuleRules
{
	public SteamCompany(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Slate", "SlateCore", "AdvancedSessions", "AdvancedSteamSessions" , "AIModule" });
	}
}
