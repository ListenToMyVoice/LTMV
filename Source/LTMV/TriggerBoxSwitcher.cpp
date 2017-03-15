// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "TriggerBoxSwitcher.h"


ATriggerBoxSwitcher::ATriggerBoxSwitcher() : Super(){
    _numTimesActive = -1;
    GetCollisionComponent()->bGenerateOverlapEvents = true;
    GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATriggerBoxSwitcher::OnOverlapBegin);
    //GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &ATriggerBoxSwitcher::OnOverlapEnd);
}

void ATriggerBoxSwitcher::BeginPlay() {
    Super::BeginPlay();
    _switcher.InitSwitcher();
}

void ATriggerBoxSwitcher::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                         bool bFromSweep, const FHitResult& SweepResult) {
    _switcher.ActivateSwitcher();
    if (--_numTimesActive == 0) SetActorEnableCollision(false);
}

//void ATriggerBoxSwitcher::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
//                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
//
//}