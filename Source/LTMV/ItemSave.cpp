// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "ItemSave.h"

UItemSave::UItemSave() : Super(), _weight(0) {
    SetComponentTickEnabled(false);
}

void UItemSave::BeginPlay() {
    Super::BeginPlay();
}