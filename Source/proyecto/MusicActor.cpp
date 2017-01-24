// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "MusicActor.h"


AMusicActor::AMusicActor(const class FObjectInitializer& OI) : Super(OI) {
    _mainTrackPath = TEXT("/Game/Audio/Music/Midnight_Syndicate-3-Born_of_the_Night");
    _actionTrackPath = TEXT("/Game/Audio/Music/04_-_Black_Night_Of_Magic");

    SetMusic(EMusicType::Main);
}

void AMusicActor::BeginPlay() {
    Super::BeginPlay();
}

void AMusicActor::SetMusic(EMusicType type) {
    FString toPlay = _mainTrackPath;
    if (type == EMusicType::Main) {
        _playing = EMusicType::Main;
        ULibraryUtils::Log(toPlay);
    }
    else if (type == EMusicType::Action) {
        _playing = EMusicType::Action;
        toPlay = _actionTrackPath;
        ULibraryUtils::Log(toPlay);
    }

    static ConstructorHelpers::FObjectFinder<USoundWave> Sound(*toPlay);
    GetAudioComponent()->SetSound(Sound.Object);
    GetAudioComponent()->Play();
}