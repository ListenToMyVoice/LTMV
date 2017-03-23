// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class LTMV : ModuleRules {
    public LTMV(TargetInfo Target) {
        PublicDependencyModuleNames.AddRange(new string[] { "Core",
                                                            "CoreUObject",
                                                            "Engine",
                                                            "InputCore",
                                                            "OnlineSubsystem",
                                                            "OnlineSubsystemUtils",
                                                            "FMODStudio" });

        PrivateDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay",
                                                             "SteamVR",
                                                             "SteamVRController" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
