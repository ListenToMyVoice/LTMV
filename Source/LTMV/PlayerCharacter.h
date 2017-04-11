// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AItemActor;
class UItemTakeLeft;
class UItemTakeRight;
class UInventory;

struct ItemData {
    //GENERATED_USTRUCT_BODY()
    AItemActor* actor;
    TArray<UActorComponent*> components;
};

UCLASS()
class LTMV_API APlayerCharacter : public ACharacter {
    GENERATED_BODY()

public:
    bool _isAction;

    bool _isVisible;

    UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
    void OnShowInventory();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAudioComponent* _audioComp;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundWave* _walkSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundWave* _runSound;

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float _baseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float _baseLookUpRate;

    APlayerCharacter();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void GetOwnComponents();

    //void ActivateScenaryItem(AItemActor* item);
    //void DeactivateScenaryItem(AItemActor* item);

    UPROPERTY(EditAnywhere, Category = "Raycast")
    float RayParameter;

    void SwitchSound(USoundWave* sound, bool stop);

    UFUNCTION(BlueprintCallable, Category = "Player pool Items")
    bool IsAction();

    bool ItemFocused();

protected:
    virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

    /*************************************** ACTION MAPPINGS *************************************/
    /*********** MOVEMENT ***********/
    void MoveForward(float Val);
    void MoveRight(float Val);
    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);

    /*********** CROUCH ***********/
    void execOnStartCrouching();
    void execOnEndCrouching();

    /************** USE *************/
    void Use();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_Use(UActorComponent* component);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_Use(UActorComponent* component);

	/************** PRESS *************/
	void Press();
	UFUNCTION(Server, Reliable, WithValidation)
		void SERVER_Press(UActorComponent* component);
	UFUNCTION(NetMulticast, Reliable)
		void MULTI_Press(UActorComponent* component);

    /********** TAKE ITEM ***********/
    void TakeDropRight();
    void TakeDropLeft();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_TakeDropRight(AActor* actor);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_TakeDropRight(AActor* actor);
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_TakeDropLeft(AActor* actor);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_TakeDropLeft(AActor* actor);


    /********** DROP ITEM ***********/
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_DropLeft();
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_DropLeft();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_DropRight();
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_DropRight();

    /*************INVENTORY************/
    // The class that will be used for the players Inventory UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    TAssetSubclassOf<class UInventoryWidget> InventoryUIClass;

    // The instance of the players Inventory UI Widget
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    class UInventoryWidget* InventoryWidget;

    UFUNCTION(BlueprintCallable, Category = "Player pool Items")
    void ShowInventory();
    void ShowInventory_Implementation(UInventory* inventory);

    void HideInventory();
    void HideInventory_Implementation();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    UInventory* GetInventory();

    UFUNCTION(BlueprintCallable, Category = "Player pool Items")
    UTexture2D* GetItemAt(int itemIndex);

    void SaveInventory(AActor* itemActor);

    void PickItemFromInventory();
    void PickItemFromInventory_Implementation(FString name);

    void SetHUDVisible(bool visible);
    bool IsHUDVisible();




    /* RAYCASTING */
    UFUNCTION(BlueprintCallable, Category = "Raycasting")
    bool RayCastCamera(FHitResult &hitActor);

    /* RAYCASTING */
    UFUNCTION(BlueprintCallable, Category = "Raycasting")
    FHitResult Raycasting();

private:
    UCameraComponent* _PlayerCamera;


    AActor* _itemLeft;
    AActor* _itemRight;
    UInventory* _inventory;
    //TArray<AItemActor*> _activeScenaryItems;
    UActorComponent* _component;

    

    //ItemData FindItemAndComponents(const TSubclassOf<UActorComponent> ComponentClass);

    //Global HitResult to check actor in every tick:
    FHitResult hitResult;

    bool bInventoryItemHit = false;

    TArray<UActorComponent*> components;

    UStaticMeshComponent* lastMeshFocused = nullptr;
    bool _itemFocused;
    bool _itemLeftTaken;
};