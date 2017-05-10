// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "PlayerSpectator.h"

#include "MenuInteraction.h"

APlayerSpectator::APlayerSpectator(const FObjectInitializer& OI) : Super(OI){
    _PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
    _PlayerCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    _PlayerCamera->bUsePawnControlRotation = true;

    _MenuInteractionComp = CreateDefaultSubobject<UMenuInteraction>(TEXT("Menu Interaction"));
    _MenuInteractionComp->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform);
    _MenuInteractionComp->_RayParameter = 100000;
}

void APlayerSpectator::ToggleMenuInteraction(bool Activate) {
    _MenuInteractionComp->SetHiddenInGame(!Activate, true);
    _MenuInteractionComp->SetComponentTickEnabled(Activate);
    _MenuInteractionComp->SetVisibility(Activate, true);
}

/******** USE ITEM LEFT *********/
void APlayerSpectator::ClickLeftPressed() {
    _MenuInteractionComp->PressPointer();
}

void APlayerSpectator::ClickLeftReleased() {
    _MenuInteractionComp->ReleasePointer();
}