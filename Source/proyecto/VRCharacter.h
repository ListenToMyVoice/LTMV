// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"

class USteamVRChaperoneComponent;

UCLASS()
class PROYECTO_API AVRCharacter : public ACharacter {
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "VR")
    bool bPositionalHeadTracking;

    /************************************* MAIN COMPONENTS ***************************************/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent* VROriginComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UCameraComponent* CameraComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USteamVRChaperoneComponent* ChaperoneComp;

    /*********************************** MOTION CONTROLLERS **************************************/
    /************ LEFT ***********/
    /* Motion Controllers */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UMotionControllerComponent* LeftHandComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent* SM_LeftHand;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UArrowComponent* LeftArrow;
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    //class USPlineComponent* LeftSPline;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USphereComponent* LeftSphere;
    /*********** RIGHT ***********/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UMotionControllerComponent* RightHandComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent* SM_RightHand;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UArrowComponent* RightArrow;
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    //class USPlineComponent* RightSPline;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USphereComponent* RightSphere;

    /********************************** ACTION MAPPINGS ******************************************/
    /* MOVEMENT */
    void MoveForward(float Val);
    void MoveRight(float Val);
    //void TurnAtRate(float Rate);
    //void LookUpAtRate(float Rate);

    /************** TRIGGER LEFT *************/
    void TriggerLeft();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_TriggerLeft(UActorComponent* component);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_TriggerLeft(UActorComponent* component);

    /************** TRIGGER RIGHT *************/
    void TriggerRight();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_TriggerRight(UActorComponent* component);
    UFUNCTION(NetMulticast, Reliable)
    void MULTI_TriggerRight(UActorComponent* component);

public:
    AVRCharacter();
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    void SetupVROptions();
    void ResetHMDOrigin();
    /* Toggle between Seated and Standing VR Tracking */
    void ToggleTrackingSpace();

private:
    IHeadMountedDisplay* HMD;

    void BuildLeft();
    void BuildRight();
};
