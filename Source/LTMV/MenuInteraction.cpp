// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "MenuInteraction.h"

#include "InputMenu.h"

UMenuInteraction::UMenuInteraction() {
    PrimaryComponentTick.bCanEverTick = true;

    _RayParameter = 5000.0f;
    _TargetLocked = false;
}

void UMenuInteraction::BeginPlay() {
    Super::BeginPlay();

    SetActive(false);
    SetComponentTickEnabled(false);
}

void UMenuInteraction::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    FHitResult HitResult;
    FCollisionQueryParams CollisionInfo;

    FVector StartRaycast = GetComponentLocation();
    FVector EndRaycast = GetForwardVector() * _RayParameter + StartRaycast;

    DrawDebugLine(GetWorld(), StartRaycast, EndRaycast, FColor(0, 255, 0), false, -1.0f, (uint8)'\000', 0.8f);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, StartRaycast, EndRaycast,
                                             ECollisionChannel::ECC_Visibility, CollisionInfo) &&
        HitResult.Actor.IsValid()) {

        if (_TargetInputMenu && HitResult.GetComponent() != _TargetInputMenu) {
            _TargetInputMenu->EndhoverInteraction();
            _TargetLocked = false;
        }

        _TargetInputMenu = Cast<UInputMenu>(HitResult.GetComponent());
        if (_TargetInputMenu && !_TargetLocked) {
            _TargetInputMenu->HoverInteraction();
            _TargetLocked = true;
        }
    }
    else {
        if (_TargetInputMenu) {
            _TargetInputMenu->EndhoverInteraction();
            _TargetInputMenu = nullptr;
            _TargetLocked = false;
        }
    }
}

void UMenuInteraction::PressPointer() {
    if (_TargetInputMenu) _TargetInputMenu->PressEvents();
}

void UMenuInteraction::ReleasePointer() {
    if (_TargetInputMenu) _TargetInputMenu->ReleaseEvents();
}