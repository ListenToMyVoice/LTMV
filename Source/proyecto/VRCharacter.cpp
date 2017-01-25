// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "VRCharacter.h"
/* VR Includes */
#include "HeadMountedDisplay.h"
#include "MotionControllerComponent.h"

AVRCharacter::AVRCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    VROriginComp = CreateDefaultSubobject<USceneComponent>(TEXT("VRCameraOrigin"));
    VROriginComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    /* Assign to the VR origin component so any reset calls to the HMD can reset to 0,0,0 relative to this component */
    CameraComp->AttachToComponent(VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);

    LeftHandComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
    LeftHandComponent->Hand = EControllerHand::Left;
    LeftHandComponent->AttachToComponent(VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);

    RightHandComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
    RightHandComponent->Hand = EControllerHand::Right;
    RightHandComponent->AttachToComponent(VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);

    bPositionalHeadTracking = false;
}

void AVRCharacter::BeginPlay() {
    Super::BeginPlay();

    SetupVROptions();
}

void AVRCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
    Super::SetupPlayerInputComponent(InputComponent);

    InputComponent->BindAction("ToggleTrackingSpace", IE_Pressed, this, &AVRCharacter::ToggleTrackingSpace);
    InputComponent->BindAction("ResetHMDOrigin", IE_Pressed, this, &AVRCharacter::ResetHMDOrigin);
}

void AVRCharacter::SetupVROptions() {
    IHeadMountedDisplay* HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());
    if (HMD && HMD->IsStereoEnabled()) {
        /* Disable/Enable positional movement to pin camera translation */
        HMD->EnablePositionalTracking(bPositionalHeadTracking);

        /* Remove any translation when disabling positional head tracking */
        if (!bPositionalHeadTracking) {
            CameraComp->SetRelativeLocation(FVector(0, 0, 0));
        }
    }
}

void AVRCharacter::ResetHMDOrigin() {
    IHeadMountedDisplay* HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());
    if (HMD && HMD->IsStereoEnabled()) {
        HMD->ResetOrientationAndPosition();
    }
}

void AVRCharacter::ToggleTrackingSpace() {
    // TODO: Fix module includes for SteamVR

    //@todo Make this safe once we can add something to the DeviceType enum.  For now, make the terrible assumption this is a SteamVR device.
    // 	FSteamVRHMD* SteamVRHMD = (FSteamVRHMD*)(GEngine->HMDDevice.Get());
    // 	if (SteamVRHMD && SteamVRHMD->IsStereoEnabled())
    // 	{
    // 		ESteamVRTrackingSpace TrackingSpace = SteamVRHMD->GetTrackingSpace();
    // 		SteamVRHMD->SetTrackingSpace(TrackingSpace == ESteamVRTrackingSpace::Seated ? ESteamVRTrackingSpace::Standing : ESteamVRTrackingSpace::Seated);
    // 	}
}