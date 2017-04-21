// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class LTMV_API APlayerCharacter : public ACharacter {
    GENERATED_BODY()

public:
    bool _isVisible;

    /************* INVENTORY ************/
    // The class that will be used for the players Inventory UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    TSubclassOf<class UInventoryWidget> InventoryUIClass;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    class UInventory* GetInventory();

    UFUNCTION(BlueprintCallable, Category = "Player pool Items")
    UTexture2D* GetItemAt(int itemIndex);

    void SaveInventory(AActor* itemActor);

    /******** USE ITEM LEFT *********/
    void UseLeftPressed();
    void UseLeftReleased();

    /******* USE ITEM RIGHT *********/
    void UseRightPressed();
    void UseRightReleased();

    /************** PICK ITEM *************/
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void PickItemFromInventory(AActor* ItemActor, FKey KeyStruct);
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_PickItemFromInventory(AActor* ItemActor, FKey KeyStruct);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_PickItemFromInventory(AActor* ItemActor, FKey KeyStruct);

    void SetHUDVisible(bool visible);
    bool IsHUDVisible();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
    int _Health;

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float _baseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float _baseLookUpRate;

    APlayerCharacter();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category = "Raycast")
    float RayParameter;

	bool ItemFocused();

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                             class AController* EventInstigator, class AActor* DamageCauser) override;

    UFUNCTION(NetMulticast, Reliable)
    void MULTI_CharacterDead();

    /* Radio Delegate */
    FRadioDelegate _OnRadioPressedDelegate;
    FRadioDelegate _OnRadioReleasedDelegate;

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
    void UsePressed();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_UsePressed(UActorComponent* component);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_UsePressed(UActorComponent* component);

    void UseReleased();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_UseReleased(UActorComponent* component);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_UseReleased(UActorComponent* component);


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

    /* RAYCASTING */
    UFUNCTION(BlueprintCallable, Category = "Raycasting")
    FHitResult Raycasting();

private:
    UPROPERTY(Category = Audio, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UFMODAudioComponent* _StepsAudioComp;
    UPROPERTY(Category = Audio, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UWidgetInteractionComponent* _WidgetInteractionComp;
    
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* _PlayerCamera;

    bool _isAction;
    AActor* _itemLeft;
    AActor* _itemRight;
    UInventory* _inventory;
    UActorComponent* _component;

     //Global HitResult to check actor in every tick:
    FHitResult hitResult;

    bool bInventoryItemHit = false;

    TArray<UActorComponent*> components;

    UStaticMeshComponent* lastMeshFocused = nullptr;
    bool _itemFocused;
    bool _itemLeftTaken;

    /* Radio Delegate */
    FDelegateHandle _OnRadioPressedDelegateHandle;
    FDelegateHandle _OnRadioReleasedDelegateHandle;
    void AddRadioDelegates(AActor* Actor);
    void ClearRadioDelegates(AActor* Actor);

public:
    FORCEINLINE UFMODAudioComponent* APlayerCharacter::GetStepsAudioComp() const { return _StepsAudioComp; }
    FORCEINLINE UWidgetInteractionComponent* APlayerCharacter::GetWidgetInteractionComp() const { return _WidgetInteractionComp; }
    FORCEINLINE UCameraComponent* APlayerCharacter::GetPlayerCamera() const { return _PlayerCamera; }
};