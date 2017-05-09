// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "VRCharacter.h"

#include "ItfUsable.h"
#include "ItfUsableItem.h"
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

    _VROriginComp = CreateDefaultSubobject<USceneComponent>(TEXT("_VROriginComp"));
    _VROriginComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    _PlayerCamera->AttachToComponent(_VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);
    _MenuInteractionComp->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform);
    _ChaperoneComp = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("_ChaperoneComp"));

    HMD = nullptr;

    BuildLeft();
    BuildRight();

    _RightSphere->OnComponentBeginOverlap.AddDynamic(this, &AVRCharacter::OnOverlap);
    _LeftSphere->OnComponentEndOverlap.AddDynamic(this, &AVRCharacter::OnEndOverlap);
}

void AVRCharacter::BuildLeft() {
    _LeftHandComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("_LeftHandComp"));
    _LeftHandComp->Hand = EControllerHand::Left;
    _LeftHandComp->AttachToComponent(_VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);

    /* MESH */
    _SM_LeftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("_SM_LeftHand"));
    _SM_LeftHand->AttachToComponent(_LeftHandComp, FAttachmentTransformRules::KeepRelativeTransform);

    /* ADDITIONAL */
    _LeftSphere = CreateDefaultSubobject<USphereComponent>(TEXT("_LeftSphere"));
    _LeftSphere->AttachToComponent(_SM_LeftHand, FAttachmentTransformRules::KeepRelativeTransform);
}

void AVRCharacter::BuildRight() {
    _RightHandComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("_RightHandComp"));
    _RightHandComp->Hand = EControllerHand::Right;
    _RightHandComp->AttachToComponent(_VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);

    /* MESH */
    _SM_RightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("_SM_RightHand"));
    _SM_RightHand->AttachToComponent(_RightHandComp, FAttachmentTransformRules::KeepRelativeTransform);

    /* ADDITIONAL */
    _RightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("_RightSphere"));
    _RightSphere->AttachToComponent(_SM_RightHand, FAttachmentTransformRules::KeepRelativeTransform);
}

void AVRCharacter::BeginPlay() {
    Super::BeginPlay();

    HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());
    //HMD->EnableHMD(true);
    //HMD->EnableStereo(true);
    SetupVROptions();
}

void AVRCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInput) {
    Super::SetupPlayerInputComponent(PlayerInput);

    /* VR SPECIFIC */
    PlayerInput->BindAction("ToggleTrackingSpace", IE_Pressed, this, &AVRCharacter::ToggleTrackingSpace);
    PlayerInput->BindAction("ResetHMDOrigin", IE_Pressed, this, &AVRCharacter::ResetHMDOrigin);
}

void AVRCharacter::SetupVROptions() {
    if (HMD && HMD->IsStereoEnabled()) {
        HMD->EnablePositionalTracking(bPositionalHeadTracking);
        /* Remove any translation when disabling positional head tracking */
        if (!bPositionalHeadTracking) _PlayerCamera->SetRelativeLocation(FVector(0, 0, 0));
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

/************** OVERLAPPING *************/
void AVRCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                             bool bFromSweep, const FHitResult& SweepResult) {

    if (SweepResult.Actor.IsValid()) {
        TArray<UActorComponent*> Components = OtherActor->GetComponentsByClass(
                                                                    UActorComponent::StaticClass());

        UStaticMeshComponent* _StaticMesh;
        bool HitItem = false;
        for (UActorComponent* Component : Components) {
            //Highlight outline colors:
            //GREEN: 252 | BLUE: 253 | ORANGE: 254 | WHITE: 255
            if (Component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                _StaticMesh = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(
                    UStaticMeshComponent::StaticClass()));

                if (_StaticMesh) {
                    _StaticMesh->SetRenderCustomDepth(true);
                    _StaticMesh->SetCustomDepthStencilValue(252);
                    HitItem = true;
                }
            }
            else if (Component->GetClass() == UGrabItem::StaticClass()) {
                _StaticMesh = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(
                    UStaticMeshComponent::StaticClass()));

                if (_StaticMesh) {
                    _StaticMesh->SetRenderCustomDepth(true);
                    _StaticMesh->SetCustomDepthStencilValue(255);
                    HitItem = true;
                }
            }
        }
        if (HitItem) {
            if (OverlappedComponent == _LeftSphere) {
                ULibraryUtils::Log("OnOverlap Left");
                _ActorFocusedLeft = OtherActor;
            }
            else if (OverlappedComponent == _RightSphere) {
                ULibraryUtils::Log("OnOverlap Right");
                _ActorFocusedRight = OtherActor;
            }
        }
    }
}

void AVRCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    if (OtherActor == _ActorFocusedLeft) {
        ULibraryUtils::Log("OnEndOverlap Left");
        _ActorFocusedLeft = nullptr;
    }
    else if (OtherActor == _ActorFocusedRight) {
        ULibraryUtils::Log("OnEndOverlap Right");
        _ActorFocusedRight = nullptr;
    }

    UStaticMeshComponent* _StaticMesh = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));
    if (_StaticMesh) {
        _StaticMesh->SetCustomDepthStencilValue(0);
        _StaticMesh->SetRenderCustomDepth(false);
    }
}

/****************************************** ACTION MAPPINGS **************************************/
/************** USE *************/
void AVRCharacter::UsePressed() {}

void AVRCharacter::UseReleased() {}

/******** USE ITEM LEFT *********/
void AVRCharacter::UseLeftPressed(bool IsMenuHidden) {
    if (IsMenuHidden) {
        if (_ItemLeft) {
            TArray<UActorComponent*> Components;
            _ItemLeft->GetComponents(Components);

            for (UActorComponent* Component : Components) {
                if (Component->GetClass()->ImplementsInterface(UItfUsableItem::StaticClass())) {
                    IItfUsableItem* ItfObject = Cast<IItfUsableItem>(Component);
                    if (ItfObject) ItfObject->Execute_UseItemPressed(Component);
                }
            }
        }
        else if (_ActorFocusedLeft) UseTriggerPressed(_ActorFocusedLeft);
    }
    else _MenuInteractionComp->PressPointer();
}

void AVRCharacter::UseLeftReleased(bool IsMenuHidden) {
    if (IsMenuHidden) {
        if (_ItemLeft) {
            TArray<UActorComponent*> Components;
            _ItemLeft->GetComponents(Components);

            for (UActorComponent* Component : Components) {
                if (Component->GetClass()->ImplementsInterface(UItfUsableItem::StaticClass())) {
                    IItfUsableItem* ItfObject = Cast<IItfUsableItem>(Component);
                    if (ItfObject) ItfObject->Execute_UseItemReleased(Component);
                }
            }
        }
        else if (_ActorFocusedLeft) UseTriggerReleased(_ActorFocusedLeft);
    }
    else _MenuInteractionComp->ReleasePointer();
}

/******* USE ITEM RIGHT *********/
void AVRCharacter::UseRightPressed(bool IsMenuHidden) {
    if (_ItemRight) {
        TArray<UActorComponent*> Components;
        _ItemRight->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsableItem::StaticClass())) {
                IItfUsableItem* ItfObject = Cast<IItfUsableItem>(Component);
                if (ItfObject) ItfObject->Execute_UseItemPressed(Component);
            }
        }
    }
    else if(_ActorFocusedRight) UseTriggerPressed(_ActorFocusedRight);
}

void AVRCharacter::UseRightReleased(bool IsMenuHidden) {
    if (_ItemRight) {
        TArray<UActorComponent*> Components;
        _ItemRight->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsableItem::StaticClass())) {
                IItfUsableItem* ItfObject = Cast<IItfUsableItem>(Component);
                if (ItfObject) ItfObject->Execute_UseItemReleased(Component);
            }
        }
    }
    else if (_ActorFocusedRight) UseTriggerReleased(_ActorFocusedRight);
}

/*************** USE TRIGGER *************/
void AVRCharacter::UseTriggerPressed(AActor* ActorFocused) {

}

void AVRCharacter::UseTriggerReleased(AActor* ActorFocused) {
    ///* OVERLAPPING DETECTION */
    //bool found = false;
    //TArray<AActor*> Actors;
    //SphereOverlap->GetOverlappingActors(Actors);
    //int i = Actors.Num() - 1;

    ///* CAN BE GRABBED */
    //while (!found && i >= 0) {
    //    if (Cast<AStaticMeshActor>(Actors[i])) {
    //        const TSet <UActorComponent*> Set = Actors[i]->GetComponents();
    //        for (UActorComponent* component : Set) {
    //            if (component->IsA(UGrabItem::StaticClass())) {
    //                SERVER_GrabItem(true, Cast<UGrabItem>(component));
    //                found = true;
    //            }
    //        }
    //    }
    //    i--;
    //}
}







































///************** TRIGGER LEFT *************/
//void AVRCharacter::TriggerLeft() {
//    /* OVERLAPPING DETECTION */
//    bool found = false;
//    TArray<AActor*> actors;
//    LeftSphere->GetOverlappingActors(actors);
//    int i = actors.Num() - 1;
//
//    if (!_itemLeft) {
//        /* CAN BE GRABBED */
//        while (!found && i >= 0) {
//            if (Cast<AStaticMeshActor>(actors[i])) {
//                const TSet <UActorComponent*> set = actors[i]->GetComponents();
//                for (UActorComponent* component : set) {
//                    if (component->IsA(UGrabItem::StaticClass())) {
//                        SERVER_GrabItem(true, Cast<UGrabItem>(component));
//                        found = true;
//                    }
//                }
//            }
//            i--;
//        }
//
//        /* CAN BE USED */
//        i = actors.Num() - 1;
//        while (!found && i >= 0) {
//            const TSet <UActorComponent*> set = actors[i]->GetComponents();
//            for (UActorComponent* component : set) {
//                if (component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
//                    SERVER_Use(component);
//                    found = true;
//                }
//            }
//            i--;
//        }
//    }
//    else {// DROP ITEM
//        SERVER_DropItem(true);
//    }
//}
//
///************** TRIGGER RIGHT *************/
//void AVRCharacter::TriggerRight() {
//    /* OVERLAPPING DETECTION */
//    bool found = false;
//    TArray<AActor*> actors;
//    RightSphere->GetOverlappingActors(actors);
//    int i = actors.Num() - 1;
//
//    if (!_itemRight) {
//        /* CAN BE GRABBED */
//        while (!found && i >= 0) {
//            if (Cast<AStaticMeshActor>(actors[i])) {
//                const TSet <UActorComponent*> set = actors[i]->GetComponents();
//                for (UActorComponent* component : set) {
//                    if (component->IsA(UGrabItem::StaticClass())) {
//                        SERVER_GrabItem(false, Cast<UGrabItem>(component));
//                        found = true;
//                    }
//                }
//            }
//            i--;
//        }
//
//        /* CAN BE USED */
//        while (!found && i >= 0) {
//            const TSet <UActorComponent*> set = actors[i]->GetComponents();
//            for (UActorComponent* component : set) {
//                if (component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
//                    SERVER_Use(component);
//                    found = true;
//                }
//            }
//            i--;
//        }
//    }
//    else {// DROP ITEM
//        SERVER_DropItem(false);
//    }
//}
//
///****************************************** ACTIONS **********************************************/
///*************** USE TRIGGER *************/
//bool AVRCharacter::SERVER_Use_Validate(UActorComponent* component) { return true; }
//void AVRCharacter::SERVER_Use_Implementation(UActorComponent* component) {
//    MULTI_Use(component);
//}
//
//void AVRCharacter::MULTI_Use_Implementation(UActorComponent* component) {
//    IItfUsable* itfObject = Cast<IItfUsable>(component);
//    if (itfObject) itfObject->Execute_UsePressed(component);
//}
//
///************** GRAB TRIGGER *************/
//bool AVRCharacter::SERVER_GrabItem_Validate(bool isLeft, UGrabItem* grabComp) { return true; }
//void AVRCharacter::SERVER_GrabItem_Implementation(bool isLeft, UGrabItem* grabComp) {
//    MULTI_GrabItem_Implementation(isLeft, grabComp);
//}
//
//void AVRCharacter::MULTI_GrabItem_Implementation(bool isLeft, UGrabItem* grabComp) {
//    UStaticMeshComponent* mesh = nullptr;
//    if (isLeft) {
//        _itemLeft = Cast<AStaticMeshActor>(grabComp->GetOwner());
//        mesh = _itemLeft->GetStaticMeshComponent();
//        mesh->SetSimulatePhysics(false);
//        mesh->AttachToComponent(SM_LeftHand,
//                                FAttachmentTransformRules::KeepRelativeTransform,
//                                TEXT("itemHand"));
//        mesh->RelativeLocation = grabComp->_locationAttach_L;
//        mesh->RelativeRotation = grabComp->_rotationAttach_L;
//        _itemLeft->SetActorEnableCollision(false);
//    }
//    else {
//        _itemRight = Cast<AStaticMeshActor>(grabComp->GetOwner());
//        mesh = _itemRight->GetStaticMeshComponent();
//        mesh->SetSimulatePhysics(false);
//        mesh->AttachToComponent(SM_RightHand,
//                                FAttachmentTransformRules::KeepRelativeTransform,
//                                TEXT("itemHand"));
//        mesh->RelativeLocation = grabComp->_locationAttach_R;
//        mesh->RelativeRotation = grabComp->_rotationAttach_R;
//        _itemRight->SetActorEnableCollision(false);
//    }
//    UE_LOG(LogTemp, Warning, TEXT("GRAB ITEM"));
//}
//
///************** DROP TRIGGER *************/
//bool AVRCharacter::SERVER_DropItem_Validate(bool isLeft) { return true; }
//void AVRCharacter::SERVER_DropItem_Implementation(bool isLeft) {
//    MULTI_DropItem_Implementation(isLeft);
//}
//
//void AVRCharacter::MULTI_DropItem_Implementation(bool isLeft) {
//    UStaticMeshComponent* mesh = nullptr;
//    if (isLeft) {
//        mesh = _itemLeft->GetStaticMeshComponent();
//        mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
//        mesh->SetSimulatePhysics(true);
//        _itemLeft->SetActorEnableCollision(true);
//        _itemLeft = nullptr;
//    }
//    else {
//        mesh = _itemRight->GetStaticMeshComponent();
//        mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
//        mesh->SetSimulatePhysics(true);
//        _itemRight->SetActorEnableCollision(true);
//        _itemRight = nullptr;
//    }
//    UE_LOG(LogTemp, Warning, TEXT("DROP ITEM"));
//}