// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


DECLARE_DELEGATE_OneParam(FPickRadioDelegate, UActorComponent*);
DECLARE_EVENT_OneParam(APlayerCharacter, FPickRadioEvent, UActorComponent*);

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
    FDelegateHandle SubscribeRadio(FPickRadioDelegate& PickRadioDelegate);
    void UnsubscribeRadio(FDelegateHandle DelegateHandle);
    bool _isVisible;

    UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
    void OnShowInventory();

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

    void GetOwnComponents();

    UPROPERTY(EditAnywhere, Category = "Raycast")
    float RayParameter;

	bool ItemFocused();
    UFUNCTION(BlueprintCallable, Category = "Player pool Items")
    bool IsAction();

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                             class AController* EventInstigator, class AActor* DamageCauser) override;

    UFUNCTION(NetMulticast, Reliable)
    void MULTI_CharacterDead();

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

    /******** USE ITEM LEFT *********/
    void UseLeftPressed();
    void UseLeftReleased();

    /******* USE ITEM RIGHT *********/
    void UseRightPressed();
    void UseRightReleased();

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
    FPickRadioEvent _PickRadioEvent;

    UPROPERTY(Category = Audio, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UFMODAudioComponent* _StepsAudioComp;
    
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

public:
    FORCEINLINE UFMODAudioComponent* APlayerCharacter::GetStepsAudioComp() const { return _StepsAudioComp; }
    FORCEINLINE UCameraComponent* APlayerCharacter::GetPlayerCamera() const { return _PlayerCamera; }
};