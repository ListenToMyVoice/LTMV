// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "GrabItem.h"


UGrabItem::UGrabItem() : Super() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UGrabItem::BeginPlay() {
    Super::BeginPlay();
}