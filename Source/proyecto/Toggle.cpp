// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "Toggle.h"


// Sets default values for this component's properties
UToggle::UToggle()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}

void UToggle::ToggleOn(bool &bProperty) {
    bProperty = true;
}

void UToggle::ToggleOff(bool &bProperty) {
    bProperty = false;
}

void UToggle::InvertState(bool &bProperty) {
    bProperty = !bProperty;
}

bool UToggle::IsToggleActive(bool bProperty) {
    return bProperty;
}

bool UToggle::Use_Implementation() {
    return true;
}

bool UToggle::Usable_Implementation(bool &bProperty) {
    if (!IsToggleActive(bProperty)) {
        ToggleOn(bProperty);
    }
    else {
        ToggleOff(bProperty);
    }
    return true;
}