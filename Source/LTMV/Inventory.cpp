// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Inventory.h"

UInventory::UInventory() {
    _items = {};
}

void UInventory::BeginPlay() {
    Super::BeginPlay();
}

int UInventory::AddItem(AActor* item) {
    ULibraryUtils::SetActorEnable(item, false);

    UStaticMeshComponent* itemMesh = Cast<UStaticMeshComponent>(item->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));

    itemMesh->AttachToComponent(this,
        FAttachmentTransformRules::KeepRelativeTransform);

    itemMesh->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
    itemMesh->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);

    _items.Add(item);

    //OnItemAdd();

    return 0;
}

TArray<AActor*> UInventory::GetItemsArray() {
    return _items;
}

void UInventory::ShowAllItems() {
    for (AActor* item:_items){
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ITEM: %s"), *item->GetName()));
    }
}


//void OnItemAdd_Implementation() {}
