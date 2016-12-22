// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "itemAttachable.h"

UitemAttachable::UitemAttachable() : Super() {}

void UitemAttachable::BeginPlay() {
    Super::BeginPlay();
}

void UitemAttachable::activateItem(UPrimitiveComponent* OverlappedComp,
                                   APlayerCharacter* OtherActor,
                                   UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult) {

    Super::activateItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex,
                        bFromSweep, SweepResult);
    
    ULibraryUtils::Log(TEXT("Attachable active"));
    _binding = &OtherActor->InputComponent->BindAction("Take", IE_Released, this,
                                                       &UitemAttachable::inputCB);
}

void UitemAttachable::deactivateItem(UPrimitiveComponent* OverlappedComp,
                                     APlayerCharacter* OtherActor, UPrimitiveComponent* OtherComp,
                                     int32 OtherBodyIndex) {

    Super::deactivateItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

    ULibraryUtils::Log(TEXT("Attachable deactive"));
    _binding->ActionDelegate.Unbind();
}

void UitemAttachable::inputCB() {
    AStaticMeshActor* owner = Cast<AStaticMeshActor>(GetOwner());
    if (owner != nullptr) {
        _actor->TakeItem(owner->GetStaticMeshComponent());
    }
}