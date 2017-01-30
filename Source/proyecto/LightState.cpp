// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "LightState.h"


ULightState::ULightState() {
    PrimaryComponentTick.bCanEverTick = true;
    _intensity = 0.0f;
    MAX_INTENSITY = 10.0f;
}

void ULightState::BeginPlay() {
    Super::BeginPlay();

    AddIntensity();
}

int ULightState::SwitchState_Implementation() {
    AddIntensity();
    return 0;
}

void ULightState::AddIntensity() {
    ULightComponent* lightComp = Cast<ULightComponent>(GetOwner()->GetComponentByClass(
        ULightComponent::StaticClass()));

    lightComp->SetIntensity(++_intensity);
    _intensity = _intensity >= MAX_INTENSITY ? 0.0f : _intensity;
}