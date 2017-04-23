// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "FPCharacter.generated.h"


UCLASS()
class LTMV_API AFPCharacter : public APlayerCharacter {
    GENERATED_BODY()

public:
    AFPCharacter();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    /************* INVENTORY ************/
    // The class that will be used for the players Inventory UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    TSubclassOf<class UInventoryWidget> InventoryUIClass;

    UFUNCTION(BlueprintCallable, Category = "Player pool Items")
    UTexture2D* GetItemAt(int itemIndex);

    /************** PICK ITEM *************/
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void PickItemInventory(AActor* ItemActor, FKey KeyStruct);
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_PickItemInventory(AActor* ItemActor, FKey KeyStruct);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_PickItemInventory(AActor* ItemActor, FKey KeyStruct);

    /******** USE ITEM LEFT *********/
    void UseLeftPressed() override;
    void UseLeftReleased() override;

    /******* USE ITEM RIGHT *********/
    void UseRightPressed() override;
    void UseRightReleased() override;

protected:
    void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

    /*************************************** ACTION MAPPINGS *************************************/
    /************** USE *************/
    void UsePressed() override;
    void UseReleased() override;

    /********** TAKE ITEM ***********/
    void TakeDropRight() override;
    void TakeDropLeft() override;

    /********** INVENTORY ***********/
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_SaveItemInventory(AActor* Actor);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_SaveItemInventory(AActor* Actor);

    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_SaveItemInventoryLeft();
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_SaveItemInventoryLeft();

    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_SaveItemInventoryRight();
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_SaveItemInventoryRight();

    AActor* GetItemFocused();

    /* RAYCASTING */
    UFUNCTION(BlueprintCallable, Category = "Raycasting")
    FHitResult Raycasting();

private:
    float _RayParameter;
    FHitResult hitResult;

    class UInventory* _Inventory;
    
    UStaticMeshComponent* lastMeshFocused = nullptr;
    bool bInventoryItemHit = false;
};
