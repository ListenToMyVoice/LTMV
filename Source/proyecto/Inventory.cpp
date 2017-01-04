// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "Inventory.h"

UInventory::UInventory() {
    //PrimaryComponentTick.bCanEverTick = true;
    _items = {};
}

void UInventory::BeginPlay() {
    Super::BeginPlay();
}

int UInventory::AddItem(AItem* item) {
    ULibraryUtils::setActorEnable(item, false);

    item->GetStaticMeshComponent()->AttachToComponent(this,
        FAttachmentTransformRules::KeepRelativeTransform);

    item->GetStaticMeshComponent()->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
    item->GetStaticMeshComponent()->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);

    _items.Add(item);
    return 0;
}