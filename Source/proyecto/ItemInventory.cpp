// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "ItemInventory.h"

UItemInventory::UItemInventory() : Super(), _weight(0) {}

void UItemInventory::BeginPlay() {
    Super::BeginPlay();
}

void UItemInventory::activateItem(UPrimitiveComponent* OverlappedComp,
                                   APlayerCharacter* OtherActor,
                                   UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult) {

    Super::activateItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex,
                        bFromSweep, SweepResult);

    ULibraryUtils::Log(TEXT("Inventory active"));
    _binding = &OtherActor->InputComponent->BindAction("Take", IE_Released, this,
                                                       &UItemInventory::inputCB);
}

void UItemInventory::deactivateItem(UPrimitiveComponent* OverlappedComp,
                                     APlayerCharacter* OtherActor, UPrimitiveComponent* OtherComp,
                                     int32 OtherBodyIndex) {

    Super::deactivateItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

    ULibraryUtils::Log(TEXT("Attachable deactive"));
    _binding->ActionDelegate.Unbind();
}

void UItemInventory::inputCB() {
    AStaticMeshActor* owner = Cast<AStaticMeshActor>(GetOwner());
    if (owner != nullptr) {
        _actor->SaveItem(owner);
    }
}