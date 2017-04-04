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
                                                            "AIModule",
                                                            "GameplayTasks" });

        PrivateDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay",
                                                             "SteamVR",
                                                             "SteamVRController",
                                                             "FMODStudio" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
