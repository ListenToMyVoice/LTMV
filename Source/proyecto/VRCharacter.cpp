// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "VRCharacter.h"
//#include "Runtime/Engine/Classes/Components/SplineComponent.h"
/* VR Includes */
#include "HeadMountedDisplay.h"
#include "MotionControllerComponent.h"
#include "SteamVRChaperoneComponent.h"

AVRCharacter::AVRCharacter() {
    PrimaryActorTick.bCanEverTick = true;
    bPositionalHeadTracking = true;

    GetCharacterMovement()->MaxWalkSpeed = 240.0f;
    GetCharacterMovement()->MaxFlySpeed = 240.0f;
    GetCharacterMovement()->MaxCustomMovementSpeed = 240.0f;
    GetCharacterMovement()->MaxWalkSpeedCrouched = 120.0f;
    GetCharacterMovement()->MaxSwimSpeed = 120.0f;

    VROriginComp = CreateDefaultSubobject<USceneComponent>(TEXT("VRCameraOrigin"));
    VROriginComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComp->AttachToComponent(VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);

    ChaperoneComp = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("ChaperoneComponent"));

    BuildLeft();
    BuildRight();
}

void AVRCharacter::BuildLeft() {
    LeftHandComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
    LeftHandComp->Hand = EControllerHand::Left;
    LeftHandComp->AttachToComponent(VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);

    /* MESH */
    SM_LeftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_LeftHand"));
    SM_LeftHand->AttachToComponent(LeftHandComp, FAttachmentTransformRules::KeepRelativeTransform);

    /* ADDITIONAL */
    LeftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArrow"));
    LeftArrow->AttachToComponent(SM_LeftHand, FAttachmentTransformRules::KeepRelativeTransform);

    //LeftSPline = CreateDefaultSubobject<USPlineComponent>(TEXT("LeftSPline"));
    //LeftSPline->AttachToComponent(SM_LeftHand, FAttachmentTransformRules::KeepRelativeTransform);

    LeftSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftSphere"));
    LeftSphere->AttachToComponent(SM_LeftHand, FAttachmentTransformRules::KeepRelativeTransform);
}

void AVRCharacter::BuildRight() {
    RightHandComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
    RightHandComp->Hand = EControllerHand::Right;
    RightHandComp->AttachToComponent(VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);

    /* MESH */
    SM_RightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_RightHand"));
    SM_RightHand->AttachToComponent(RightHandComp, FAttachmentTransformRules::KeepRelativeTransform);

    /* ADDITIONAL */
    RightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrow"));
    RightArrow->AttachToComponent(SM_RightHand, FAttachmentTransformRules::KeepRelativeTransform);

    //RightSPline = CreateDefaultSubobject<USPlineComponent>(TEXT("RightSPline"));
    //RightSPline->AttachToComponent(SM_RightHand, FAttachmentTransformRules::KeepRelativeTransform);

    RightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightSphere"));
    RightSphere->AttachToComponent(SM_RightHand, FAttachmentTransformRules::KeepRelativeTransform);
}

void AVRCharacter::BeginPlay() {
    Super::BeginPlay();

    HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());
    //HMD->EnableHMD(true);
    //HMD->EnableStereo(true);
    SetupVROptions();
}

void AVRCharacter::SetupPlayerInputComponent(class UInputComponent* playerInput) {
    Super::SetupPlayerInputComponent(playerInput);

    /* MOVEMENT */
    playerInput->BindAxis("VRThumbLeft_Y", this, &AVRCharacter::MoveForward);
    playerInput->BindAxis("VRThumbLeft_X", this, &AVRCharacter::MoveRight);

    playerInput->BindAction("ToggleTrackingSpace", IE_Pressed, this, &AVRCharacter::ToggleTrackingSpace);
    playerInput->BindAction("ResetHMDOrigin", IE_Pressed, this, &AVRCharacter::ResetHMDOrigin);
}

void AVRCharacter::SetupVROptions() {
    if (HMD && HMD->IsStereoEnabled()) {
        HMD->EnablePositionalTracking(bPositionalHeadTracking);
        /* Remove any translation when disabling positional head tracking */
        if (!bPositionalHeadTracking) CameraComp->SetRelativeLocation(FVector(0, 0, 0));
    }
    ResetHMDOrigin();
}

void AVRCharacter::ResetHMDOrigin() {// R
    if (HMD && HMD->IsStereoEnabled()) HMD->ResetOrientationAndPosition();
}

void AVRCharacter::ToggleTrackingSpace() {// T
    // TODO: Fix module includes for SteamVR

    //@todo Make this safe once we can add something to the DeviceType enum.  For now, make the terrible assumption this is a SteamVR device.
    //FSteamVRHMD* SteamVRHMD = (FSteamVRHMD*)(GEngine->HMDDevice.Get());
    //if (SteamVRHMD && SteamVRHMD->IsStereoEnabled())
    //{
    // 	ESteamVRTrackingSpace TrackingSpace = SteamVRHMD->GetTrackingSpace();
    // 	SteamVRHMD->SetTrackingSpace(TrackingSpace == ESteamVRTrackingSpace::Seated ? ESteamVRTrackingSpace::Standing : ESteamVRTrackingSpace::Seated);
    //}
}

/****************************************** ACTION MAPPINGS **************************************/
/*********** MOVEMENT ***********/
void AVRCharacter::MoveForward(float Value) {
    if (Value != 0.0f) {
        AddMovementInput(CameraComp->GetForwardVector(), Value);
    }
}

void AVRCharacter::MoveRight(float Value) {
    if (Value != 0.0f) {
        AddMovementInput(CameraComp->GetRightVector(), Value);
    }
}

//void APlayerCharacter::TurnAtRate(float Rate) {
//    AddControllerYawInput(Rate * _baseTurnRate * GetWorld()->GetDeltaSeconds());
//}
//
//void APlayerCharacter::LookUpAtRate(float Rate) {
//    AddControllerPitchInput(Rate * _baseLookUpRate * GetWorld()->GetDeltaSeconds());
//}