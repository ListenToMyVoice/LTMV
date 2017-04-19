// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "UseSwitcher.h"

UUseSwitcher::UUseSwitcher() : Super() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UUseSwitcher::BeginPlay() {
    Super::BeginPlay();
    _switcher.InitSwitcher();
}

void UUseSwitcher::UsePressed_Implementation() {
    _switcher.ActivateSwitcher();
}


void UUseSwitcher::UseReleased_Implementation() {
	_switcher.ActivateSwitcher();
}
