// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "MenuInteraction.h"

#include "InputMenu.h"
#include "Menu3D.h"

UMenuInteraction::UMenuInteraction() {
    PrimaryComponentTick.bCanEverTick = true;
    bAutoActivate = false;

    _RayParameter = 500.0f;
    _TargetLocked = false;

    _Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
    _Light->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
    _Light->SetOuterConeAngle(10);
    _Light->SetInnerConeAngle(5);
    _Light->SetIntensity(2500);

    FLinearColor Color = FLinearColor();
    Color.R = 0.635;
    Color.G = 0.635;
    Color.B = 0.635;
    _Light->SetLightColor(Color, true);
}

void UMenuInteraction::BeginPlay() {
    Super::BeginPlay();

    SetActive(false);
    SetComponentTickEnabled(false);
    SetHiddenInGame(true, true);
    SetVisibility(false, true);

    if (GetOwner()) _Light = Cast<USpotLightComponent>(GetOwner()->GetComponentByClass(USpotLightComponent::StaticClass()));
}

void UMenuInteraction::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FHitResult HitResult;
    FCollisionQueryParams CollisionInfo;

    FVector StartRaycast = GetComponentLocation();
    FVector EndRaycast = GetForwardVector() * _RayParameter + StartRaycast;

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
        _Light->SetIntensity(10);
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