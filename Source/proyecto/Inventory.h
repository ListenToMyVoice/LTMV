// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"

#include "Components/StaticMeshComponent.h"
#include "Inventory.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROYECTO_API UInventory : public UStaticMeshComponent {
    GENERATED_BODY()
private:
    TArray<AItem*> _items;

public:
    UInventory();
    virtual void BeginPlay() override;

    int AddItem(AItem* item);

    //virtual void TickComponent(float DeltaTime, ELevelTick TickType,
    //                           FActorComponentTickFunction* ThisTickFunction) override;
};
