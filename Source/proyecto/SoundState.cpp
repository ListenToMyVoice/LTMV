// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "SoundState.h"


USoundState::USoundState() {
    PrimaryComponentTick.bCanEverTick = true;
}

void USoundState::BeginPlay() {
    Super::BeginPlay();
}

int USoundState::SwitchState_Implementation() {
    Play();
    return 0;
}