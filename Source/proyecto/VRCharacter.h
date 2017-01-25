// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"

UCLASS()
class PROYECTO_API AVRCharacter : public ACharacter {
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UCameraComponent* CameraComp;

    /* Component to specify origin for the HMD */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* VROriginComp;

    UPROPERTY(EditDefaultsOnly, Category = "VR")
    bool bPositionalHeadTracking;

    /* Motion Controllers */
    UPROPERTY(EditDefaultsOnly, Category = "Components")
    class UMotionControllerComponent* LeftHandComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    class UMotionControllerComponent* RightHandComponent;

public:
    AVRCharacter();
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    void SetupVROptions();
    void ResetHMDOrigin();
    /* Toggle between Seated and Standing VR Tracking */
    void ToggleTrackingSpace();
};
