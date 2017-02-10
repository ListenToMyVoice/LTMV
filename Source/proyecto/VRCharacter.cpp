// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "VRCharacter.h"
//#include "Runtime/Engine/Classes/Components/SplineComponent.h"
/* VR Includes */
#include "HeadMountedDisplay.h"
#include "MotionControllerComponent.h"

AVRCharacter::AVRCharacter() {
    PrimaryActorTick.bCanEverTick = true;
    bPositionalHeadTracking = false;

    VROriginComp = CreateDefaultSubobject<USceneComponent>(TEXT("VRCameraOrigin"));
    VROriginComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    /* Assign to the VR origin component so any reset calls to the HMD can reset to 0,0,0 relative to this component */
    CameraComp->AttachToComponent(VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);
    GetMesh()->AttachToComponent(CameraComp, FAttachmentTransformRules::KeepRelativeTransform, TEXT("FPVCamera"));

    BuildLeft();
    BuildRight();
}

void AVRCharacter::BuildLeft() {
    LeftHandComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
    LeftHandComponent->Hand = EControllerHand::Left;
    LeftHandComponent->AttachToComponent(VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);

    /* MESH */
    SM_LeftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_LeftHand"));
    SM_LeftHand->AttachToComponent(LeftHandComponent, FAttachmentTransformRules::KeepRelativeTransform);

    /* ADDITIONAL */
    LeftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArrow"));
    LeftArrow->AttachToComponent(SM_LeftHand, FAttachmentTransformRules::KeepRelativeTransform);

    //LeftSPline = CreateDefaultSubobject<USPlineComponent>(TEXT("LeftSPline"));
    //LeftSPline->AttachToComponent(SM_LeftHand, FAttachmentTransformRules::KeepRelativeTransform);

    LeftSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftSphere"));
    LeftSphere->AttachToComponent(SM_LeftHand, FAttachmentTransformRules::KeepRelativeTransform);
}

void AVRCharacter::BuildRight() {
    RightHandComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
    RightHandComponent->Hand = EControllerHand::Right;
    RightHandComponent->AttachToComponent(VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);

    /* MESH */
    SM_RightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_RightHand"));
    SM_RightHand->AttachToComponent(RightHandComponent, FAttachmentTransformRules::KeepRelativeTransform);

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
    SetupVROptions();

    CameraComp->SetRelativeLocation(FVector(0, 0, -110));
    //CameraComp->SetRelativeRotation(FRotator(-90, 0, 90));

    //GetMesh()->SetRelativeLocation(FVector(-20, 0, -162));
    //GetMesh()->SetRelativeRotation(FRotator(0, 0, -90));
}

void AVRCharacter::SetupPlayerInputComponent(class UInputComponent* playerInput) {
    Super::SetupPlayerInputComponent(playerInput);

    /* MOVEMENT */
    //playerInput->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    //playerInput->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    playerInput->BindAxis("VRThumbLeft_Y", this, &AVRCharacter::MoveForward);
    playerInput->BindAxis("VRThumbLeft_X", this, &AVRCharacter::MoveRight);

    playerInput->BindAction("ToggleTrackingSpace", IE_Pressed, this, &AVRCharacter::ToggleTrackingSpace);
    playerInput->BindAction("ResetHMDOrigin", IE_Pressed, this, &AVRCharacter::ResetHMDOrigin);
}

void AVRCharacter::SetupVROptions() {
    IHeadMountedDisplay* HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());
    HMD->ResetOrientationAndPosition();
    //HMD->EnableHMD(true);
    //HMD->EnableStereo(true);
    ULibraryUtils::Log(TEXT("SetupVROptions"));
    if (HMD/* && HMD->IsStereoEnabled()*/) {
        /* Disable/Enable positional movement to pin camera translation */
        HMD->EnablePositionalTracking(bPositionalHeadTracking);
        ULibraryUtils::Log(TEXT("EnablePositionalTracking"));
        /* Remove any translation when disabling positional head tracking */
        if (!bPositionalHeadTracking) {
            CameraComp->SetRelativeLocation(FVector(0, 0, 0));
        }
    }
}

void AVRCharacter::ResetHMDOrigin() {// R
    IHeadMountedDisplay* HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());
    if (HMD && HMD->IsStereoEnabled()) {
        ULibraryUtils::Log(TEXT("ResetOrientationAndPosition"));
        HMD->ResetOrientationAndPosition();
    }
}

void AVRCharacter::ToggleTrackingSpace() {// T
    // TODO: Fix module includes for SteamVR

    //@todo Make this safe once we can add something to the DeviceType enum.  For now, make the terrible assumption this is a SteamVR device.
    // 	FSteamVRHMD* SteamVRHMD = (FSteamVRHMD*)(GEngine->HMDDevice.Get());
    // 	if (SteamVRHMD && SteamVRHMD->IsStereoEnabled())
    // 	{
    // 		ESteamVRTrackingSpace TrackingSpace = SteamVRHMD->GetTrackingSpace();
    // 		SteamVRHMD->SetTrackingSpace(TrackingSpace == ESteamVRTrackingSpace::Seated ? ESteamVRTrackingSpace::Standing : ESteamVRTrackingSpace::Seated);
    // 	}
}

/****************************************** ACTION MAPPINGS **************************************/
/*********** MOVEMENT ***********/
void AVRCharacter::MoveForward(float Value) {
    if (Value != 0.0f) {
        AddMovementInput(GetActorForwardVector(), Value);
        //ULibraryUtils::Log(TEXT("MoveForward"));
    }
}

void AVRCharacter::MoveRight(float Value) {
    if (Value != 0.0f) {
        AddMovementInput(GetActorRightVector(), Value);
        //ULibraryUtils::Log(TEXT("MoveRight"));
    }
}

//void APlayerCharacter::TurnAtRate(float Rate) {
//    AddControllerYawInput(Rate * _baseTurnRate * GetWorld()->GetDeltaSeconds());
//}
//
//void APlayerCharacter::LookUpAtRate(float Rate) {
//    AddControllerPitchInput(Rate * _baseLookUpRate * GetWorld()->GetDeltaSeconds());
//}