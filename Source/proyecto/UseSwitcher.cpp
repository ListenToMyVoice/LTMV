// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "UseSwitcher.h"

UUseSwitcher::UUseSwitcher() : Super() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UUseSwitcher::BeginPlay() {
    Super::BeginPlay();
    _switcher.InitSwitcher();
}

void UUseSwitcher::Use_Implementation() {
    _switcher.ActivateSwitcher();
}


void UUseSwitcher::Press_Implementation() {
	_switcher.ActivateSwitcher();
}
