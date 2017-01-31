// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "LightState.h"


ULightState::ULightState() {
    PrimaryComponentTick.bCanEverTick = true;
    MAX_INTENSITY = 10.0f;
}

void ULightState::BeginPlay() {
    Super::BeginPlay();
}

int ULightState::SwitchState_Implementation() {
    ULightComponent* lightComp = Cast<ULightComponent>(GetOwner()->GetComponentByClass(
        ULightComponent::StaticClass()));

    float i = lightComp->Intensity + _increment;
    i = i > MAX_INTENSITY ? 0 : i;
    lightComp->SetIntensity(i);
    return 0;
}