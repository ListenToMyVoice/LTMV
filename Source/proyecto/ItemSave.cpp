// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "ItemSave.h"

UItemSave::UItemSave() : Super(), _weight(0) {}

void UItemSave::BeginPlay() {
    Super::BeginPlay();
}

void UItemSave::activateItem(UPrimitiveComponent* OverlappedComp,
                                   APlayerCharacter* OtherActor,
                                   UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult) {

    Super::activateItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex,
                        bFromSweep, SweepResult);

    ULibraryUtils::Log(TEXT("Inventory active"));
    _binding = &OtherActor->InputComponent->BindAction("Save", IE_Released, this,
                                                       &UItemSave::inputCB);
}

void UItemSave::deactivateItem(UPrimitiveComponent* OverlappedComp,
                                     APlayerCharacter* OtherActor, UPrimitiveComponent* OtherComp,
                                     int32 OtherBodyIndex) {

    Super::deactivateItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

    ULibraryUtils::Log(TEXT("Inventory deactive"));
    _binding->ActionDelegate.Unbind();
}

void UItemSave::inputCB() {
    AStaticMeshActor* owner = Cast<AStaticMeshActor>(GetOwner());
    if (owner != nullptr) {
        _actor->SaveItem(owner);
    }
}