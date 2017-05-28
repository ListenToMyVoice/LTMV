// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "WallTrapState.h"


UWallTrapState::UWallTrapState() {
    PrimaryComponentTick.bCanEverTick = true;

    _State = EWallState::NoPressed;
}

void UWallTrapState::BeginPlay() {
    Super::BeginPlay();
}

void UWallTrapState::TickComponent(float DeltaTime, ELevelTick TickType,
                                   FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int UWallTrapState::SwitchState_Implementation(int Param) {
    return 0;
}