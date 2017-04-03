// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Inventory.h"

UInventory::UInventory() {
    _items = {};
}

void UInventory::BeginPlay() {
    Super::BeginPlay();
}

int UInventory::AddItem(AItemActor* item) {
    ULibraryUtils::SetActorEnable(item, false);

    item->GetStaticMeshComponent()->AttachToComponent(this,
        FAttachmentTransformRules::KeepRelativeTransform);

    item->GetStaticMeshComponent()->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
    item->GetStaticMeshComponent()->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);

    _items.Add(item);
    return 0;
}