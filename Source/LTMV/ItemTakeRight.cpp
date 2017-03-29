// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "ItemTakeRight.h"
#include "ItemActor.h"
#include "PlayerCharacter.h"

UItemTakeRight::UItemTakeRight() : Super(), _locationAttach(0.f, 0.f, 0.f),
                                            _rotationAttach(0.f, 0.f, 0.f) {}

void UItemTakeRight::BeginPlay() {
    Super::BeginPlay();
}

void UItemTakeRight::activateItem(UPrimitiveComponent* OverlappedComp,
                                 APlayerCharacter* player,
                                 UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult) {

    Super::activateItem(OverlappedComp, player, OtherComp, OtherBodyIndex, bFromSweep,
                        SweepResult);

    AItemActor* owner = Cast<AItemActor>(GetOwner());
    //player->ActivateScenaryItem(owner);
}

void UItemTakeRight::deactivateItem(UPrimitiveComponent* OverlappedComp,
                                   APlayerCharacter* player, UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex) {

    Super::deactivateItem(OverlappedComp, player, OtherComp, OtherBodyIndex);

    AItemActor* owner = Cast<AItemActor>(GetOwner());
    //player->DeactivateScenaryItem(owner);
}