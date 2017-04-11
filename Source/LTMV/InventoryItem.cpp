// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "InventoryItem.h"


// Sets default values for this component's properties
UInventoryItem::UInventoryItem() : Super(), _weight(0){
    SetComponentTickEnabled(false);
    //_itemImage = UTexture2D::GetAssetUserData()
}


// Called when the game starts
void UInventoryItem::BeginPlay()
{
	Super::BeginPlay();	
}

void UInventoryItem::SetEquipped(bool equipped) {
    _equipped = equipped;
}

bool UInventoryItem::IsEquipped() {
    return _equipped;
}

UTexture2D* UInventoryItem::GetItemImage() {
    return _itemImage;
}


