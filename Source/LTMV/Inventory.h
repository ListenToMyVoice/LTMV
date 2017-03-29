// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemActor.h"

#include "Components/StaticMeshComponent.h"
#include "Inventory.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LTMV_API UInventory : public UStaticMeshComponent {
    GENERATED_BODY()
private:
    TArray<AActor*> _items;

public:
    UInventory();
    virtual void BeginPlay() override;

    int AddItem(AActor* item);

    //virtual void TickComponent(float DeltaTime, ELevelTick TickType,
    //                           FActorComponentTickFunction* ThisTickFunction) override;
};
