// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "TextureState.h"


// Sets default values for this component's properties
UTextureState::UTextureState() {
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTextureState::BeginPlay() {
    Super::BeginPlay();
}


int UTextureState::SwitchState_Implementation() {
    //    ULightComponent* lightComp = Cast<ULightComponent>(GetOwner()->GetComponentByClass(
    //        ULightComponent::StaticClass()));
    //
    //    float i = lightComp->Intensity + _increment;
    //    i = i > MAX_INTENSITY ? 0 : i;
    //        i = i > MAX_INTENSITY ? 0 : i;
    //    lightComp->SetIntensity(i);
    return 0;
}
