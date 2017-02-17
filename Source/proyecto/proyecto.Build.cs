// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class proyecto : ModuleRules {
    public proyecto(TargetInfo Target) {
        PublicDependencyModuleNames.AddRange(new string[] { "Core",
                                                            "CoreUObject",
                                                            "Engine",
                                                            "InputCore",
                                                            "OnlineSubsystem",
                                                            "OnlineSubsystemUtils" });

        PrivateDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay",
                                                             "SteamVR",
                                                             "SteamVRController" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
