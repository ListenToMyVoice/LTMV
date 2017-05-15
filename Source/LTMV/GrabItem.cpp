// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "GrabItem.h"


UGrabItem::UGrabItem() : Super(), _locationAttach_L(0.f, 0.f, 0.f),
                                  _rotationAttach_L(0.f, 0.f, 0.f),
                                  _locationAttach_R(0.f, 0.f, 0.f), 
                                  _rotationAttach_R(0.f, 0.f, 0.f),
                                  _locationAttach_C(0.f, 0.f, 0.f),
                                  _rotationAttach_C(0.f, 0.f, 0.f) {
    _IsBeingTaked = false;
    _SourceMesh = nullptr;
    _TargetMesh = nullptr;
    _SocketName = "";
}

void UGrabItem::BeginPlay() {
	Super::BeginPlay();
    _SourceMesh = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(
                                                            UStaticMeshComponent::StaticClass()));
}

void UGrabItem::BeginGrab(USceneComponent* Target, FName SocketName) {
    _TargetMesh = Cast<UStaticMeshComponent>(Target);
    if (_SourceMesh && _TargetMesh) {
        _IsBeingTaked = true;

        _SourceMesh->SetMobility(EComponentMobility::Movable);
        _SourceMesh->SetSimulatePhysics(false);

        SetComponentTickEnabled(true);
    }
}

void UGrabItem::EndGrab() {
    SetComponentTickEnabled(false);

    if (_SourceMesh) {
        _SourceMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        _SourceMesh->SetSimulatePhysics(true);
    }
    _TargetMesh = nullptr;
    _IsBeingTaked = false;
}

void UGrabItem::TickComponent(float DeltaTime, ELevelTick TickType,
                              FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (_IsBeingTaked) {
        ULibraryUtils::Log("TickComponent");
        FVector NewLocation = FMath::VInterpConstantTo(_SourceMesh->GetSocketLocation(_SocketName),
                                                       _TargetMesh->GetComponentLocation(),
                                                       DeltaTime, 1);

        _SourceMesh->SetWorldLocation(NewLocation);
    }
}