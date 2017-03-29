// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Toggle.h"

#include "UnrealNetwork.h"


// Sets default values for this component's properties
UToggle::UToggle()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}

TArray<AActor*> UToggle::GetArrayOfActors() {
    return OtherActors;
}

TArray<UActorComponent*> UToggle::GetArrayOfComponents() {
    return OtherComponents;
}

TArray<bool> UToggle::GetArrayOfStates() {
    return OtherActorsStates;
}

void UToggle::SetState(bool NewState, int index) {
    OtherActorsStates[index] = NewState;
}

bool UToggle::ToggleOn() {
    return true;
}

bool UToggle::ToggleOff() {
    return false;
}

bool UToggle::InvertState(bool bProperty) {
    bProperty = !bProperty;
    return bProperty;
}