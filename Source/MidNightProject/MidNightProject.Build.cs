// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MidNightProject : ModuleRules
{
	public MidNightProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "HTTP", "JsonUtilities", "Json", "UMG", "Slate", "SlateCore", "RuntimeAudioImporter" });
	}
}
