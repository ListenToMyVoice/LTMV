// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LibraryUtils.generated.h"

/**
 * Utility Class
 */
UCLASS()
class PROYECTO_API ULibraryUtils : public UObject {
    GENERATED_BODY()
public:
    ULibraryUtils(const FObjectInitializer& OI);
    /*
    Modes:
        0: Info
        1: Warning
        2: Error
        3: Debug
    */
    static FORCEINLINE void Log(const FString &msg, int mode = 0, float displayTime = 5.f) {
        #if UE_BUILD_DEVELOPMENT
        switch (mode) {
            case 1: // Warning
                GEngine->AddOnScreenDebugMessage(-1, displayTime, FColor::Yellow, msg);
                break;
            case 2: // Error
                GEngine->AddOnScreenDebugMessage(-1, displayTime, FColor::Red, msg);
                break;
            case 3: // Debug
                GEngine->AddOnScreenDebugMessage(-1, displayTime, FColor::White, msg);
                break;
            default: // Info
                GEngine->AddOnScreenDebugMessage(-1, displayTime, FColor::Green, msg);
        }
        #endif
    } 
};