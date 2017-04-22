// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class LTMV_API APlayerCharacter : public ACharacter {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
    int _Health;

    /************* INVENTORY ************/
    // The class that will be used for the players Inventory UI
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    TSubclassOf<class UInventoryWidget> InventoryUIClass;

    UFUNCTION(BlueprintCallable, Category = "Player pool Items")
    UTexture2D* GetItemAt(int itemIndex);

    /******** USE ITEM LEFT *********/
    void UseLeftPressed();
    void UseLeftReleased();

    /******* USE ITEM RIGHT *********/
    void UseRightPressed();
    void UseRightReleased();

    /************** PICK ITEM *************/
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void PickItemInventory(AActor* ItemActor, FKey KeyStruct);
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_PickItemInventory(AActor* ItemActor, FKey KeyStruct);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_PickItemInventory(AActor* ItemActor, FKey KeyStruct);

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

	AActor* GetItemFocused();

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
    void SERVER_TakeRight(AActor* Actor);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_TakeRight(AActor* Actor);
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_TakeLeft(AActor* Actor);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_TakeLeft(AActor* Actor);

    
    /********** DROP ITEM ***********/
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_DropLeft();
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_DropLeft();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_DropRight();
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_DropRight();


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

    bool _IsAction;
    AActor* _ItemLeft;
    AActor* _ItemRight;
    UInventory* _Inventory;

     //Global HitResult to check actor in every tick:
    FHitResult hitResult;

    bool bInventoryItemHit = false;

    TArray<UActorComponent*> components;

    UStaticMeshComponent* lastMeshFocused = nullptr;
    bool _ItemFocused;

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