// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "LightOn.h"


ULightOn::ULightOn() : Super() {}

void ULightOn::BeginPlay() {
    Super::BeginPlay();
}

void ULightOn::activateItem(UPrimitiveComponent* OverlappedComp,
                                  APlayerCharacter* player,
                                  UPrimitiveComponent* OtherComp,
                                  int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult) {

    Super::activateItem(OverlappedComp, player, OtherComp, OtherBodyIndex, bFromSweep,
                        SweepResult);

    AItemActor* owner = Cast<AItemActor>(GetOwner());
    player->ActivateScenaryItem(owner);
}

void ULightOn::deactivateItem(UPrimitiveComponent* OverlappedComp,
                                    APlayerCharacter* player, UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex) {

    Super::deactivateItem(OverlappedComp, player, OtherComp, OtherBodyIndex);

    AItemActor* owner = Cast<AItemActor>(GetOwner());
    player->DeactivateScenaryItem(owner);
}

int ULightOn::Use_Implementation() {
    ULibraryUtils::Log(TEXT("LIGHT ON"));
    return 0;
}