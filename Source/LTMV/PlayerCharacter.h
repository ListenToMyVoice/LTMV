// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class LTMV_API APlayerCharacter : public ACharacter {
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float _baseTurnRate;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    float _baseLookUpRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life")
    int _Health;

    /* Radio Delegate */
    FRadioDelegate _OnRadioPressedDelegate;
    FRadioDelegate _OnRadioReleasedDelegate;


    APlayerCharacter();
    virtual void AfterPossessed(bool SetInventory);

    /******** USE ITEM LEFT *********/
    virtual void UseLeftPressed(bool IsMenuHidden);
    virtual void UseLeftReleased(bool IsMenuHidden);

    /******* USE ITEM RIGHT *********/
    virtual void UseRightPressed(bool IsMenuHidden);
    virtual void UseRightReleased(bool IsMenuHidden);

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                             class AController* EventInstigator, class AActor* DamageCauser) override;
    
    /*********** MOVEMENT ***********/
    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);

    AActor* GetWalkieActor();

protected:
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* _PlayerCamera;
    UPROPERTY(Category = Audio, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UFMODAudioComponent* _StepsAudioComp;
    UPROPERTY(Category = Audio, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UWidgetInteractionComponent* _WidgetInteractionComp;

    AActor* _ItemLeft;
    AActor* _ItemRight;

    void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

    /*************************************** ACTION MAPPINGS *************************************/
    /*********** MOVEMENT ***********/
    void MoveForward(float Val);
    void MoveRight(float Val);

    /************** USE *************/
    virtual void UsePressed();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_UsePressed(UActorComponent* component);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_UsePressed(UActorComponent* component);

    virtual void UseReleased();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_UseReleased(UActorComponent* component);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_UseReleased(UActorComponent* component);


    /********** TAKE ITEM ***********/
    virtual void TakeDropRight();
    virtual void TakeDropLeft();
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
    void SERVER_Drop(AActor* HandPointer);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_Drop(AActor* HandPointer);

    UFUNCTION(NetMulticast, Reliable)
    void MULTI_CharacterDead();

    UFUNCTION(Client, Reliable)
    void CLIENT_AddRadioDelegates(AActor* Actor);
    UFUNCTION(Client, Reliable)
    void CLIENT_ClearRadioDelegates(AActor* Actor);

private:
    AActor* _WalkieActor;

    /* Radio Delegate */
    FDelegateHandle _OnRadioPressedDelegateHandle;
    FDelegateHandle _OnRadioReleasedDelegateHandle;

public:
    FORCEINLINE UCameraComponent* APlayerCharacter::GetPlayerCamera() const { return _PlayerCamera; }
    FORCEINLINE UFMODAudioComponent* APlayerCharacter::GetStepsAudioComp() const { return _StepsAudioComp; }
    FORCEINLINE UWidgetInteractionComponent* APlayerCharacter::GetWidgetInteractionComp() const { return _WidgetInteractionComp; }
};