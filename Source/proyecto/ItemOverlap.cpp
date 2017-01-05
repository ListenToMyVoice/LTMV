// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "ItemOverlap.h"

UItemOverlap::UItemOverlap() : Super(), _player(nullptr) {
    PrimaryComponentTick.bCanEverTick = true;
    SetComponentTickEnabled(false);
}

void UItemOverlap::BeginPlay() {
    Super::BeginPlay();
}

void UItemOverlap::TickComponent(float DeltaTime, ELevelTick TickType,
                                 FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UItemOverlap::activateItem(UPrimitiveComponent* OverlappedComp,
                                APlayerCharacter* OtherActor,
                                UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult) {
    
    //SetComponentTickEnabled(true);
    _player = OtherActor;
}

void UItemOverlap::deactivateItem(UPrimitiveComponent* OverlappedComp,
                                  APlayerCharacter* OtherActor, UPrimitiveComponent* OtherComp,
                                  int32 OtherBodyIndex) {

    //SetComponentTickEnabled(false);
    _player = nullptr;
}