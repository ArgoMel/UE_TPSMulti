// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class CyanSdkEditor : ModuleRules
{
	public CyanSdkEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PrecompileForTargets = PrecompileTargetsType.Any;
        string pluginDirectory = Path.Combine(ModuleDirectory, "Private"); ;

        if (!Directory.Exists(pluginDirectory))
        {
            bUsePrecompiled = true;
        }
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
				"UnrealEd",
                "GraphEditor",
                "BlueprintGraph",
                "AnimGraph",
                "AnimGraphRuntime",
                "RHI",
                "CyanSdk",
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
