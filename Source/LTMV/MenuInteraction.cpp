// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "MenuInteraction.h"

#include "InputMenu.h"

UMenuInteraction::UMenuInteraction() {
    PrimaryComponentTick.bCanEverTick = true;

    _RayParameter = 5000.0f;
}

void UMenuInteraction::BeginPlay() {
    Super::BeginPlay();
}

void UMenuInteraction::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FHitResult HitResult;
    FCollisionQueryParams CollisionInfo;

    FVector StartRaycast = GetComponentLocation();
    FVector EndRaycast = GetForwardVector() * _RayParameter + StartRaycast;

    if (GetWorld()->LineTraceSingleByChannel(HitResult, StartRaycast, EndRaycast, ECC_Visibility,
                                             CollisionInfo) && HitResult.Actor.IsValid()) {
        if (_TargetInputMenu && HitResult.GetComponent() != _TargetInputMenu) {
            _TargetInputMenu->EndhoverInteraction();
        }

        _TargetInputMenu = Cast<UInputMenu>(HitResult.GetComponent());
        if (_TargetInputMenu) _TargetInputMenu->HoverInteraction();
    }
    else {
        if (_TargetInputMenu) _TargetInputMenu->EndhoverInteraction();
    }
}

void UMenuInteraction::Activation() {
    SetActive(true);
    SetComponentTickEnabled(true);
}

void UMenuInteraction::Deactivation() {
    SetActive(false);
    SetComponentTickEnabled(false);
}

void UMenuInteraction::PressPointer() {
    ULibraryUtils::Log("PressPointer");
}

void UMenuInteraction::ReleasePointer() {
    ULibraryUtils::Log("ReleasePointer");
}