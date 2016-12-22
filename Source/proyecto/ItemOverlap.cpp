// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "ItemOverlap.h"

UItemOverlap::UItemOverlap() : Super() {
    PrimaryComponentTick.bCanEverTick = false;
    _isActive = false;
    _actor = nullptr;
}

void UItemOverlap::BeginPlay() {
    Super::BeginPlay();
}

//void UItemOverlap::TickComponent(float DeltaTime, ELevelTick TickType,
//                                 FActorComponentTickFunction* ThisTickFunction) {
//    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

void UItemOverlap::activateItem(UPrimitiveComponent* OverlappedComp,
                                APlayerCharacter* OtherActor,
                                UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult) {

    _isActive = true;
    _actor = OtherActor;
}

void UItemOverlap::deactivateItem(UPrimitiveComponent* OverlappedComp,
                                  APlayerCharacter* OtherActor, UPrimitiveComponent* OtherComp,
                                  int32 OtherBodyIndex) {

    _isActive = false;
    _actor = nullptr;
}