// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "ItemTakeLeft.h"

UItemTakeLeft::UItemTakeLeft() : Super(), _locationAttach(0.f, 0.f, 0.f),
                                              _rotationAttach(0.f, 0.f, 0.f) {}

void UItemTakeLeft::BeginPlay() {
    Super::BeginPlay();
}

void UItemTakeLeft::activateItem(UPrimitiveComponent* OverlappedComp,
                                   APlayerCharacter* OtherActor,
                                   UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult) {

    Super::activateItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex,
                        bFromSweep, SweepResult);
    
    ULibraryUtils::Log(TEXT("Left active"));
    _binding = &OtherActor->InputComponent->BindAction("TakeLeft", IE_Released, this,
                                                       &UItemTakeLeft::inputCB);
}

void UItemTakeLeft::deactivateItem(UPrimitiveComponent* OverlappedComp,
                                   APlayerCharacter* OtherActor, UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex) {

    Super::deactivateItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

    ULibraryUtils::Log(TEXT("Left deactive"));
    _binding->ActionDelegate.Unbind();
}

void UItemTakeLeft::inputCB() {
    AStaticMeshActor* owner = Cast<AStaticMeshActor>(GetOwner());
    if (owner != nullptr) {
        _actor->TakeLeft(owner, _locationAttach, _rotationAttach);
    }
}