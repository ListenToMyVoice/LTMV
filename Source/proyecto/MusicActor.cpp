// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "MusicActor.h"


AMusicActor::AMusicActor(const class FObjectInitializer& OI) : Super(OI) {
    //static ConstructorHelpers::FObjectFinder<USoundWave> Sound(
    //    TEXT("/Game/Audio/Music/Midnight_Syndicate-3-Born_of_the_Night"));
    //static ConstructorHelpers::FObjectFinder<USoundWave> Sound2(
    //    TEXT("/Game/Audio/Music/04_-_Black_Night_Of_Magic"));

    //_mainTrackPath = Sound.Object;
    //_actionTrackPath = Sound2.Object;
}

void AMusicActor::BeginPlay() {
    Super::BeginPlay();

    //SetMusic(EMusicType::Main);
}

void AMusicActor::SetMusic(EMusicType type) {
    if (type == EMusicType::Main) {
        _playing = EMusicType::Main;
        GetAudioComponent()->SetSound(_mainTrackPath);
    }
    else if (type == EMusicType::Action) {
        _playing = EMusicType::Action;
        GetAudioComponent()->SetSound(_actionTrackPath);
    }
    GetAudioComponent()->Play();
}