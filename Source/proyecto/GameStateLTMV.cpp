// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "GameStateLTMV.h"
#include "MusicActor.h"


AGameStateLTMV::AGameStateLTMV(const class FObjectInitializer& OI) : Super(OI) {}

void AGameStateLTMV::BeginPlay() {
    Super::BeginPlay();
    
    //_musicActor = GetWorld()->SpawnActor<AMusicActor>(FActorSpawnParameters());
    //if (_musicActor) _musicActor->SetMusic(EMusicType::Action);
}