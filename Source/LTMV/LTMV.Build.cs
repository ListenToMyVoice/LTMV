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
                                                            "MoviePlayer",
															"GameplayTasks",
                                                            "UMG"});

        PrivateDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay",
                                                             "SteamVR",
                                                             "SteamVRController",
                                                             "FMODStudio",
                                                             "Slate",
                                                             "SlateCore" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
