// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "GrabItem.h"


UGrabItem::UGrabItem() : Super(), _locationAttach_L(0.f, 0.f, 0.f),
                                  _rotationAttach_L(0.f, 0.f, 0.f),
                                  _locationAttach_R(0.f, 0.f, 0.f), 
                                  _rotationAttach_R(0.f, 0.f, 0.f),
                                  _locationAttach_C(0.f, 0.f, 0.f),
                                  _rotationAttach_C(0.f, 0.f, 0.f) {
    
    PrimaryComponentTick.bCanEverTick = true;

    _IsBeingTaked = false;
    _SourceMesh = nullptr;
    _TargetMesh = nullptr;
    _SocketName = "";
}

void UGrabItem::BeginPlay() {
	Super::BeginPlay();
}

void UGrabItem::BeginGrab(USceneComponent* Target, FName SocketName) {
    _SourceMesh = Cast<UMeshComponent>(GetOwner()->GetComponentByClass(UMeshComponent::StaticClass()));
    _TargetMesh = Cast<UMeshComponent>(Target);
    if (_SourceMesh && _TargetMesh) {
        _IsBeingTaked = true;

        _SourceMesh->SetMobility(EComponentMobility::Movable);
        _SourceMesh->SetSimulatePhysics(false);

        SetComponentTickEnabled(true);
        ULibraryUtils::Log("BeginGrab");
    }
}

void UGrabItem::EndGrab() {
    SetComponentTickEnabled(false);
    ClearOnGrabDelegate();

    if (_SourceMesh) {
        _SourceMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        _SourceMesh->SetSimulatePhysics(true);
        ULibraryUtils::Log("EndGrab");
    }
    _TargetMesh = nullptr;
    _IsBeingTaked = false;

    _SourceMesh = nullptr;
    _TargetMesh = nullptr;
}

void UGrabItem::TickComponent(float DeltaTime, ELevelTick TickType,
                              FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (_IsBeingTaked) {
        FVector NewLocation = FMath::VInterpConstantTo(_SourceMesh->GetSocketLocation(_SocketName),
                                                       _TargetMesh->GetComponentLocation(),
                                                       DeltaTime, 3);

        _SourceMesh->SetWorldLocation(NewLocation);
        // DETECT ATTACH FIRE EVENTS
    }
}

/*********************************************** DELEGATES ***************************************/
void UGrabItem::AddOnGrabDelegate(FGrabDelegate& GrabDelegate) {
    _OnGrabItemDelegateHandle = _GrabItemEvent.Add(GrabDelegate);
}

void UGrabItem::ClearOnGrabDelegate() {
    _GrabItemEvent.Remove(_OnGrabItemDelegateHandle);
}