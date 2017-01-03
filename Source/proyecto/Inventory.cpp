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

int UInventory::addItem(AItem* itemActor) {
    _items.Add(item);
    return 0;
}