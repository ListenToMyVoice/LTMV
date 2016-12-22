// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "itemAttachable.h"

#ifndef __LIBRARYUTILS_H
#include "LibraryUtils.h"
#define __LIBRARYUTILS_H
#endif

UitemAttachable::UitemAttachable() : Super() {}

void UitemAttachable::BeginPlay() {
    Super::BeginPlay();
}

void UitemAttachable::TickComponent(float DeltaTime, ELevelTick TickType,
                                 FActorComponentTickFunction* ThisTickFunction) {
    if (_isActive) {
        Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
        ULibraryUtils::Log(TEXT("TICK"), 3);
    }
}

void UitemAttachable::activateItem(UPrimitiveComponent* OverlappedComp,
                                AActor* OtherActor,
                                UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult) {

    Super::activateItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex,
                        bFromSweep, SweepResult);
    
    ULibraryUtils::Log(TEXT("Attachable active"));
}

void UitemAttachable::deactivateItem(UPrimitiveComponent* OverlappedComp,
                           AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex) {

    Super::deactivateItem(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

    ULibraryUtils::Log(TEXT("Attachable deactive"));
}