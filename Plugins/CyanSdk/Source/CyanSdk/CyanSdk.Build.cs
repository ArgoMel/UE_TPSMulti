// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class CyanSdk : ModuleRules
{
    
    public CyanSdk(ReadOnlyTargetRules Target) : base(Target)
	{
		PrecompileForTargets = PrecompileTargetsType.Any;
		string pluginDirectory =  Path.Combine(ModuleDirectory, "Private"); ;

		if (!Directory.Exists(pluginDirectory))
        {
            bUsePrecompiled = true;
        }

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        RuntimeDependencies.Add(Path.Combine("$(ProjectDir)/Config", "CyanSetting.ini"));
        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				"CyanSdk/Public",
                "CyanSdk/Private"
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
				"WebSockets",
                "Sockets",
                "Networking",
                "InputCore",
                "Json",
                "JsonUtilities",
				"Projects",
                "HairStrandsCore",
                "Niagara"
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
