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

UCLASS(config = Game)
class LTMV_API APlayerCharacter : public ACharacter {
    GENERATED_BODY()

public:
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

    /********** TAKE LEFT ***********/
    void TakeLeft();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_TakeLeft(AActor* actor, UItemTakeLeft* takeComp);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_TakeLeft(AActor* actor, UItemTakeLeft* takeComp);
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_DropLeft();
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_DropLeft();

    /********** TAKE RIGHT ***********/
    void TakeRight();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_TakeRight(AActor* actor, UItemTakeRight* takeComp);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_TakeRight(AActor* actor, UItemTakeRight* takeComp);
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_DropRight();
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_DropRight();

    /************ SAVE LEFT **********/
    void SaveLeft();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_SaveLeft(AActor* itemActor);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_SaveLeft(AActor* itemActor);

    /************ SAVE RIGHT **********/
    void SaveRight();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_SaveRight(AActor* itemActor);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_SaveRight(AActor* itemActor);

    /*************INVENTORY************/
    void ShowInventory();
    void ShowInventory_Implementation(UInventory* inventory);


    /* RAYCASTING */
    UFUNCTION(BlueprintCallable, Category = "Raycasting")
    bool RayCastCamera(FHitResult &hitActor);

    /* RAYCASTING */
    UFUNCTION(BlueprintCallable, Category = "Raycasting")
    FHitResult Raycasting();

private:
    UCameraComponent* _PlayerCamera;

    bool _isAction;
    AActor* _itemLeft;
    AActor* _itemRight;
    UInventory* _inventory;
    //TArray<AItemActor*> _activeScenaryItems;
    UActorComponent* _component;

    void SaveInventory(AActor* itemActor);

    //ItemData FindItemAndComponents(const TSubclassOf<UActorComponent> ComponentClass);

    //Global HitResult to check actor in every tick:
    FHitResult hitResult;

    bool bInventoryItemHit = false;

    TArray<UActorComponent*> components;

    UStaticMeshComponent* lastMeshFocused = nullptr;
    bool _itemFocused;
    bool _itemLeftTaken;
};