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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UMotionControllerComponent* LeftHandComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent* SM_LeftHand;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UArrowComponent* LeftArrow;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USPlineComponent* LeftSPline;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USphereComponent* LeftSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UMotionControllerComponent* RightHandComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent* SM_RightHand;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UArrowComponent* RightArrow;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USPlineComponent* RightSPline;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USphereComponent* RightSphere;

    /********************************** ACTION MAPPINGS ******************************************/
    /* MOVEMENT */
    void MoveForward(float Val);
    void MoveRight(float Val);
    //void TurnAtRate(float Rate);
    //void LookUpAtRate(float Rate);

public:
    AVRCharacter();
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    void SetupVROptions();
    void ResetHMDOrigin();
    /* Toggle between Seated and Standing VR Tracking */
    void ToggleTrackingSpace();

private:
    void BuildLeft();
    void BuildRight();
};
