// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "SoundState.h"


USoundState::USoundState() {
    PrimaryComponentTick.bCanEverTick = true;
}

void USoundState::BeginPlay() {
    Super::BeginPlay();
}

int USoundState::SwitchStateReleased_Implementation() {
	if (_onPress) {
		Play();
	}
	return 0;
}
int USoundState::SwitchStatePressed_Implementation() {
	if (!_onPress) {
		Play();
	}
    return 0;
}