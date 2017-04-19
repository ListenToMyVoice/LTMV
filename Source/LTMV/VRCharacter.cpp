// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "VRCharacter.h"

#include "ItfUsable.h"
#include "GrabItem.h"

/* VR Includes */
#include "HeadMountedDisplay.h"
#include "MotionControllerComponent.h"
#include "SteamVRChaperoneComponent.h"

AVRCharacter::AVRCharacter(const FObjectInitializer& OI) : Super(OI) {
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

    HMD = nullptr;
    _itemLeft = nullptr;
    _itemRight = nullptr;

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

    /* ACTIONS */
    playerInput->BindAction("TriggerLeft", IE_Released, this, &AVRCharacter::TriggerLeft);
    playerInput->BindAction("TriggerRight", IE_Released, this, &AVRCharacter::TriggerRight);
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

/************** TRIGGER LEFT *************/
void AVRCharacter::TriggerLeft() {
    /* OVERLAPPING DETECTION */
    bool found = false;
    TArray<AActor*> actors;
    LeftSphere->GetOverlappingActors(actors);
    int i = actors.Num() - 1;

    if (!_itemLeft) {
        /* CAN BE GRABBED */
        while (!found && i >= 0) {
            if (Cast<AStaticMeshActor>(actors[i])) {
                const TSet <UActorComponent*> set = actors[i]->GetComponents();
                for (UActorComponent* component : set) {
                    if (component->IsA(UGrabItem::StaticClass())) {
                        SERVER_GrabItem(true, Cast<UGrabItem>(component));
                        found = true;
                    }
                }
            }
            i--;
        }

        /* CAN BE USED */
        i = actors.Num() - 1;
        while (!found && i >= 0) {
            const TSet <UActorComponent*> set = actors[i]->GetComponents();
            for (UActorComponent* component : set) {
                if (component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                    SERVER_Use(component);
                    found = true;
                }
            }
            i--;
        }
    }
    else {// DROP ITEM
        SERVER_DropItem(true);
    }
}

/************** TRIGGER RIGHT *************/
void AVRCharacter::TriggerRight() {
    /* OVERLAPPING DETECTION */
    bool found = false;
    TArray<AActor*> actors;
    RightSphere->GetOverlappingActors(actors);
    int i = actors.Num() - 1;

    if (!_itemRight) {
        /* CAN BE GRABBED */
        while (!found && i >= 0) {
            if (Cast<AStaticMeshActor>(actors[i])) {
                const TSet <UActorComponent*> set = actors[i]->GetComponents();
                for (UActorComponent* component : set) {
                    if (component->IsA(UGrabItem::StaticClass())) {
                        SERVER_GrabItem(false, Cast<UGrabItem>(component));
                        found = true;
                    }
                }
            }
            i--;
        }

        /* CAN BE USED */
        while (!found && i >= 0) {
            const TSet <UActorComponent*> set = actors[i]->GetComponents();
            for (UActorComponent* component : set) {
                if (component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                    SERVER_Use(component);
                    found = true;
                }
            }
            i--;
        }
    }
    else {// DROP ITEM
        SERVER_DropItem(false);
    }
}

/****************************************** ACTIONS **********************************************/
/*************** USE TRIGGER *************/
bool AVRCharacter::SERVER_Use_Validate(UActorComponent* component) { return true; }
void AVRCharacter::SERVER_Use_Implementation(UActorComponent* component) {
    MULTI_Use(component);
}

void AVRCharacter::MULTI_Use_Implementation(UActorComponent* component) {
    IItfUsable* itfObject = Cast<IItfUsable>(component);
    if (itfObject) itfObject->Execute_UsePressed(component);
}

/************** GRAB TRIGGER *************/
bool AVRCharacter::SERVER_GrabItem_Validate(bool isLeft, UGrabItem* grabComp) { return true; }
void AVRCharacter::SERVER_GrabItem_Implementation(bool isLeft, UGrabItem* grabComp) {
    MULTI_GrabItem_Implementation(isLeft, grabComp);
}

void AVRCharacter::MULTI_GrabItem_Implementation(bool isLeft, UGrabItem* grabComp) {
    UStaticMeshComponent* mesh = nullptr;
    if (isLeft) {
        _itemLeft = Cast<AStaticMeshActor>(grabComp->GetOwner());
        mesh = _itemLeft->GetStaticMeshComponent();
        mesh->SetSimulatePhysics(false);
        mesh->AttachToComponent(SM_LeftHand,
                                FAttachmentTransformRules::KeepRelativeTransform,
                                TEXT("itemHand"));
        mesh->RelativeLocation = grabComp->_locationAttach_L;
        mesh->RelativeRotation = grabComp->_rotationAttach_L;
        _itemLeft->SetActorEnableCollision(false);
    }
    else {
        _itemRight = Cast<AStaticMeshActor>(grabComp->GetOwner());
        mesh = _itemRight->GetStaticMeshComponent();
        mesh->SetSimulatePhysics(false);
        mesh->AttachToComponent(SM_RightHand,
                                FAttachmentTransformRules::KeepRelativeTransform,
                                TEXT("itemHand"));
        mesh->RelativeLocation = grabComp->_locationAttach_R;
        mesh->RelativeRotation = grabComp->_rotationAttach_R;
        _itemRight->SetActorEnableCollision(false);
    }
    UE_LOG(LogTemp, Warning, TEXT("GRAB ITEM"));
}

/************** DROP TRIGGER *************/
bool AVRCharacter::SERVER_DropItem_Validate(bool isLeft) { return true; }
void AVRCharacter::SERVER_DropItem_Implementation(bool isLeft) {
    MULTI_DropItem_Implementation(isLeft);
}

void AVRCharacter::MULTI_DropItem_Implementation(bool isLeft) {
    UStaticMeshComponent* mesh = nullptr;
    if (isLeft) {
        mesh = _itemLeft->GetStaticMeshComponent();
        mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        mesh->SetSimulatePhysics(true);
        _itemLeft->SetActorEnableCollision(true);
        _itemLeft = nullptr;
    }
    else {
        mesh = _itemRight->GetStaticMeshComponent();
        mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        mesh->SetSimulatePhysics(true);
        _itemRight->SetActorEnableCollision(true);
        _itemRight = nullptr;
    }
    UE_LOG(LogTemp, Warning, TEXT("DROP ITEM"));
}