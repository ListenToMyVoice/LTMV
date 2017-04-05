// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AItemActor;
class UItemTakeLeft;
class UItemTakeRight;

struct ItemData {
    //GENERATED_USTRUCT_BODY()
    AItemActor* actor;
    TArray<UActorComponent*> components;
};

UCLASS()
class LTMV_API APlayerCharacter : public ACharacter {
    GENERATED_BODY()

public:
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
    virtual void Tick(float DeltaTime) override;

    void GetOwnComponents();

    void ActivateScenaryItem(AItemActor* item);
    void DeactivateScenaryItem(AItemActor* item);

    UPROPERTY(EditAnywhere, Category = "Raycast")
    float RayParameter;

    UFUNCTION(BlueprintCallable, Category = "Player pool Items")
    bool IsAction();

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                             class AController* EventInstigator, class AActor* DamageCauser) override;

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
    void SERVER_TakeLeft(AItemActor* actor, UItemTakeLeft* takeComp);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_TakeLeft(AItemActor* actor, UItemTakeLeft* takeComp);
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_DropLeft();
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_DropLeft();

    /********** TAKE RIGHT ***********/
    void TakeRight();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_TakeRight(AItemActor* actor, UItemTakeRight* takeComp);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_TakeRight(AItemActor* actor, UItemTakeRight* takeComp);
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_DropRight();
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_DropRight();

    /************ SAVE LEFT **********/
    void SaveLeft();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_SaveLeft(AItemActor* itemActor);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_SaveLeft(AItemActor* itemActor);

    /************ SAVE RIGHT **********/
    void SaveRight();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_SaveRight(AItemActor* itemActor);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_SaveRight(AItemActor* itemActor);


    /* RAYCASTING */
    UFUNCTION(BlueprintCallable, Category = "Raycasting")
    bool RayCastCamera(FHitResult &hitActor);

    /* RAYCASTING */
    UFUNCTION(BlueprintCallable, Category = "Raycasting")
    FHitResult Raycasting();

private:
    UPROPERTY(Category = Audio, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UFMODAudioComponent* _StepsAudioComp;
    
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* _PlayerCamera;

    bool _isAction;
    AItemActor* _itemLeft;
    AItemActor* _itemRight;
    TArray<AItemActor*> _activeScenaryItems;
    UActorComponent* _component;

    void SaveInventory(AItemActor* itemActor);

    ItemData FindItemAndComponents(const TSubclassOf<UActorComponent> ComponentClass);

public:
    FORCEINLINE UFMODAudioComponent* APlayerCharacter::GetStepsAudioComp() const { return _StepsAudioComp; }
    FORCEINLINE UCameraComponent* APlayerCharacter::GetPlayerCamera() const { return _PlayerCamera; }
};