// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "ItemTakeRight.h"

UItemTakeRight::UItemTakeRight() : Super(), _locationAttach(0.f, 0.f, 0.f),
                                            _rotationAttach(0.f, 0.f, 0.f),
                                            _isAttached(false) {}

void UItemTakeRight::BeginPlay() {
    Super::BeginPlay();
}

void UItemTakeRight::activateItem(UPrimitiveComponent* OverlappedComp,
                                   APlayerCharacter* OtherActor,
                                   UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult) {
    if (!_isActive) {
        Super::activateItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex,
                            bFromSweep, SweepResult);

        ULibraryUtils::Log(TEXT("Right active"));
        _binding = &OtherActor->InputComponent->BindAction("TakeRight", IE_Released, this,
                                                           &UItemTakeRight::inputCB);
    }
}

void UItemTakeRight::deactivateItem(UPrimitiveComponent* OverlappedComp,
                                   APlayerCharacter* OtherActor, UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex) {
    if (_isActive) {
        Super::deactivateItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
        ULibraryUtils::Log(TEXT("Right deactive"));
        _binding->ActionDelegate.Unbind();
    }
}

void UItemTakeRight::inputCB() {
    AStaticMeshActor* owner = Cast<AStaticMeshActor>(GetOwner());
    _isActive = false;
    if (_isAttached) {
        // DROP
        _isAttached = false;
        _actor->DropRight(owner);
        _actor = nullptr;
    }
    else {
        // TAKE
        _isAttached = true;
        _actor->TakeRight(owner, _locationAttach, _rotationAttach);
    }
}