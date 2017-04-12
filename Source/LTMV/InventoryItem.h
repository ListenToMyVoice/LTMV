// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "InventoryItem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LTMV_API UInventoryItem : public UActorComponent{
	GENERATED_BODY()

public:	
    UPROPERTY(EditAnywhere)
    int _weight;

    UPROPERTY(EditAnywhere)
    bool _equipped;

    UPROPERTY(EditAnywhere)
    UTexture2D* _itemImage;

    UInventoryItem();

    UPROPERTY(EditAnywhere)
    FVector _locationAttachFromInventory_L;
    UPROPERTY(EditAnywhere)
    FRotator _rotationAttachFromInventory_L;

    UPROPERTY(EditAnywhere)
    FVector _locationAttachFromInventory_R;
    UPROPERTY(EditAnywhere)
    FRotator _rotationAttachFromInventory_R;

    virtual void BeginPlay() override;	

    void SetEquipped(bool equipped);
    
    bool IsEquipped();


    UFUNCTION(BlueprintCallable, Category = "InventoryItem")
    UTexture2D* GetItemImage();

private:
    
};
