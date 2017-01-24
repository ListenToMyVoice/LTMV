// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound/AmbientSound.h"
#include "MusicActor.generated.h"

UENUM(BlueprintType)
enum class EMusicType : uint8 {
    Main UMETA(DisplayName = "Main"),
    Action UMETA(DisplayName = "Action")
};

UCLASS()
class PROYECTO_API AMusicActor : public AAmbientSound {
    GENERATED_BODY()

public:
    EMusicType _playing;

    FString _mainTrackPath;
    FString _actionTrackPath;

    AMusicActor(const class FObjectInitializer& OI);
    virtual void BeginPlay() override;

    void SetMusic(EMusicType type);

private:
    USoundWave* _sound;
};
