// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "VRCharacter.h"

#include "ItfUsable.h"
#include "ItfUsableItem.h"
#include "GrabItem.h"

/* VR Includes */
#include "HeadMountedDisplay.h"
#include "IMotionController.h"
#include "MotionControllerComponent.h"
#include "SteamVRChaperoneComponent.h"


AVRCharacter::AVRCharacter(const FObjectInitializer& OI) : Super(OI) {
    PrimaryActorTick.bCanEverTick = true;
    bPositionalHeadTracking = true;

    static ConstructorHelpers::FObjectFinder<UForceFeedbackEffect> FFFinderLeft(
        TEXT("/Game/BluePrints/Effects/RumbleLightLeft"));
    _RumbleOverLapLeft = FFFinderLeft.Object;
    static ConstructorHelpers::FObjectFinder<UForceFeedbackEffect> FFFinderRight(
        TEXT("/Game/BluePrints/Effects/RumbleLightRight"));
    _RumbleOverLapRight = FFFinderRight.Object;

    GetCharacterMovement()->MaxWalkSpeed = 240.0f;
    GetCharacterMovement()->MaxFlySpeed = 240.0f;
    GetCharacterMovement()->MaxCustomMovementSpeed = 240.0f;
    GetCharacterMovement()->MaxWalkSpeedCrouched = 120.0f;
    GetCharacterMovement()->MaxSwimSpeed = 120.0f;
    
    _VROriginComp = CreateDefaultSubobject<USceneComponent>(TEXT("_VROriginComp"));
    _VROriginComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    _VROriginComp->SetRelativeLocation(FVector(15.f, 0.f, 75.f));
    //_VROriginComp->RelativeLocation.Z -= 100;
    

    GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
    GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

    _PlayerCamera->AttachToComponent(_VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);
    _ChaperoneComp = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("_ChaperoneComp"));

    _PouchLeft = CreateDefaultSubobject<USphereComponent>(TEXT("PouchLeft"));
    _PouchLeft->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Pouch1"));
    _PouchLeft->SetSphereRadius(5.0f);
    _PouchRight = CreateDefaultSubobject<USphereComponent>(TEXT("PouchRight"));
    _PouchRight->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Pouch2"));
    _PouchRight->SetSphereRadius(5.0f);

    HMD = nullptr;
    this->BaseEyeHeight = 0.f;
    this->CrouchedEyeHeight = 0.f;
    _GripStateLeft = EGripEnum::Open;
    _GripStateRight = EGripEnum::Open;
    MaxHeadTurnValue = 80.f;

    _ActorPouchLeft = nullptr;
    _ActorPouchRight = nullptr;

    BuildLeft();
    BuildRight();

    _RightSphere->OnComponentBeginOverlap.AddDynamic(this, &AVRCharacter::OnOverlap);
    _RightSphere->OnComponentEndOverlap.AddDynamic(this, &AVRCharacter::OnEndOverlap);
    _LeftSphere->OnComponentBeginOverlap.AddDynamic(this, &AVRCharacter::OnOverlap);
    _LeftSphere->OnComponentEndOverlap.AddDynamic(this, &AVRCharacter::OnEndOverlap);
}

void AVRCharacter::BuildLeft() {
    _LeftHandComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("_LeftHandComp"));
    _LeftHandComp->Hand = EControllerHand::Left;
    _LeftHandComp->AttachToComponent(_VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);
    _LeftHandComp->SetRelativeLocation(FVector(10.f, 0.f, 0.f));

    /* MESH */
    _SM_LeftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("_SM_LeftHand"));
    _SM_LeftHand->AttachToComponent(_LeftHandComp, FAttachmentTransformRules::KeepRelativeTransform);
    _SM_LeftHand->SetWorldScale3D(FVector(1.0f, 1.0f, -1.0f));
    _SM_LeftHand->SetRelativeRotation(FRotator(0.f, 0.0f, 90.f));
    _SM_LeftHand->SetRelativeLocation(FVector(-10.f, 0.f, 0.f));
    //_SM_LeftHand->SetHiddenInGame(true) ;
    //_SM_LeftHand->SetVisibility(false);

    /* ADDITIONAL */
    _LeftSphere = CreateDefaultSubobject<USphereComponent>(TEXT("_LeftSphere"));
    _LeftSphere->AttachToComponent(_SM_LeftHand, FAttachmentTransformRules::KeepRelativeTransform);
    _LeftSphere->SetRelativeLocation(FVector(20.f, 0.f, 0.f));
    _LeftSphere->SetSphereRadius(12.f);
}

void AVRCharacter::BuildRight() {
    _RightHandComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("_RightHandComp"));
    _RightHandComp->Hand = EControllerHand::Right;
    _RightHandComp->AttachToComponent(_VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);
    _RightHandComp->SetRelativeLocation(FVector(10.f, 0.f, 0.f));

    /* MESH */
    _SM_RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("_SM_RightHand"));
    _SM_RightHand->AttachToComponent(_RightHandComp, FAttachmentTransformRules::KeepRelativeTransform);
    _SM_RightHand->SetRelativeRotation(FRotator(0.f, 0.0f, 90.f));
    _SM_RightHand->SetRelativeLocation(FVector(-10.f, 0.f, 0.f));
    //_SM_RightHand->SetHiddenInGame(true);
    //_SM_RightHand->SetVisibility(false);

    /* ADDITIONAL */
    _RightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("_RightSphere"));
    _RightSphere->AttachToComponent(_SM_RightHand, FAttachmentTransformRules::KeepRelativeTransform);
    _RightSphere->SetRelativeLocation(FVector(20.f, 0.f, 0.f));
    _RightSphere->SetSphereRadius(12.f);
}

void AVRCharacter::BeginPlay() {
    Super::BeginPlay();

    HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());
    if (HMD) HMD->EnablePositionalTracking(bPositionalHeadTracking);

    if (IsPlayerControlled()) {
        APlayerController* const PC = Cast<APlayerController>(GetController());
        PC->InputYawScale = 1.0f;
    }

    if (GetMesh() != nullptr) {
        HeadCameraOffset = GetMesh()->GetBoneLocation(TEXT("head")) - _PlayerCamera->GetComponentLocation();
        BodyCameraOffset = GetMesh()->GetComponentLocation() - _PlayerCamera->GetComponentLocation();
        BodyCameraOffset.Z = GetMesh()->GetComponentLocation().Z;
    }

    bHeadTurn = false;
    bHeadTurning = false;

    _GrabDelegateLeft.BindUObject(this, &AVRCharacter::ItemGrabbedLeft);
    _GrabDelegateRight.BindUObject(this, &AVRCharacter::ItemGrabbedRight);

    CalculateMeshArmExtension();
}

void AVRCharacter::Tick(float deltaTime) {
    Super::Tick(deltaTime);

    UpdateMeshPostitionWithCamera();

    // TODO: ARREGLAR LA ROTATION SOBRE SI MISMO CUANDO SE USA EL GIRO CON MODO COMFORT
    //UpdateMeshRotationWithCamera();

    UpdateIK();
    
    SERVER_UpdateComponentPosition(_LeftHandComp, _LeftHandComp->RelativeLocation,
                                                  _LeftHandComp->RelativeRotation);

    SERVER_UpdateComponentPosition(_RightHandComp, _RightHandComp->RelativeLocation,
                                                   _RightHandComp->RelativeRotation);

}

void AVRCharacter::UpdateMeshPostitionWithCamera() {
    GetMesh()->SetWorldLocation(_PlayerCamera->GetComponentLocation() + BodyCameraOffset -
        FVector(0.f, 0.f, _PlayerCamera->GetComponentLocation().Z));
}

void AVRCharacter::UpdateMeshRotationWithCamera() {
    if (!bHeadTurn && !bHeadTurning) {
        CheckHeadTurn();
        bHeadTurn = false;
    }

    if (bHeadTurning) {
        TurnBody();
    }
}

void AVRCharacter::CheckHeadTurn() {
    float MeshYaw = GetMesh()->GetComponentRotation().Yaw + 90.f;
    float CameraYaw = _PlayerCamera->GetComponentRotation().Yaw;
    float RelativeYaw = CameraYaw - MeshYaw;

    if (RelativeYaw >= MaxHeadTurnValue || RelativeYaw <= - MaxHeadTurnValue) {
        bHeadTurn = true;
        bHeadTurning = true;
    }
}

void AVRCharacter::TurnBody() {
    float InterpSpeed = 2.f / GetWorld()->DeltaTimeSeconds;
    FRotator CurrentRotation = GetMesh()->GetComponentRotation();

    FRotator TargetRotation = FRotator(GetMesh()->GetComponentRotation().Pitch,
                                       _PlayerCamera->GetComponentRotation().Yaw - 90.f,
                                       GetMesh()->GetComponentRotation().Roll);
    
    FRotator NextRotation = FMath::RInterpConstantTo(CurrentRotation,
                                                     TargetRotation,
                                                     GetWorld()->DeltaTimeSeconds,
                                                     InterpSpeed);

    float RotationAngle = (NextRotation - CurrentRotation).Yaw;
    BodyCameraOffset = BodyCameraOffset.RotateAngleAxis(RotationAngle, _VROriginComp->GetUpVector());

    GetMesh()->SetWorldRotation(NextRotation);

    if ((NextRotation - TargetRotation).IsNearlyZero()) { bHeadTurning = false; }
}

/********** UPDATE LOCATIONS ***********/
bool AVRCharacter::SERVER_UpdateComponentPosition_Validate(USceneComponent* Component,
                                                           FVector Location,
                                                           FRotator Rotation) { return true; }
void AVRCharacter::SERVER_UpdateComponentPosition_Implementation(USceneComponent* Component,
                                                                 FVector Location,
                                                                 FRotator Rotation) {
    MULTI_UpdateComponentPosition(Component, Location, Rotation);
}
void AVRCharacter::MULTI_UpdateComponentPosition_Implementation(USceneComponent* Component,
                                                                FVector Location,
                                                                FRotator Rotation) {
    Component->SetRelativeLocationAndRotation(Location, Rotation);
}

void AVRCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInput) {
    Super::SetupPlayerInputComponent(PlayerInput);

    /* ACTIONS */
    PlayerInput->BindAction("DropLeft", IE_Pressed, this, &AVRCharacter::DropLeft);
    PlayerInput->BindAction("DropRight", IE_Pressed, this, &AVRCharacter::DropRight);

    /* VR SPECIFIC */
    PlayerInput->BindAction("ToggleTrackingSpace", IE_Pressed, this, &AVRCharacter::ToggleTrackingSpace);
    PlayerInput->BindAction("ResetHMDOrigin", IE_Pressed, this, &AVRCharacter::ResetHMDOrigin);

    /* MOVEMENT */
    PlayerInput->BindAction("TurnVRCharacter", IE_Pressed, this, &AVRCharacter::TurnVRCharacter);
}

void AVRCharacter::ResetHMDOrigin() {// R
    if (HMD) HMD->ResetOrientationAndPosition();
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

void AVRCharacter::MoveForward(float Value) {
    AddMovementInput(GetMesh()->GetRightVector(), Value);
}

void AVRCharacter::TurnVRCharacter() {
    float _CameraYawValue = _PlayerCamera->GetComponentRotation().Yaw;
    float _PlayerYawValue = GetActorRotation().Yaw;
    float _MeshYawValue = GetMesh()->GetComponentRotation().Yaw + 90.f;

    float _YawRelativeValue = _CameraYawValue - _PlayerYawValue;
    float _MeshYawRelativeValue = _CameraYawValue - _MeshYawValue;

    AddControllerYawInput(_YawRelativeValue);
    SetActorRotation(FRotator(GetActorRotation().Pitch, _YawRelativeValue, GetActorRotation().Roll));

    HMD->ResetOrientation();

    BodyCameraOffset = BodyCameraOffset.RotateAngleAxis(_MeshYawRelativeValue, _VROriginComp->GetUpVector());

    // TODO: ARREGLAR QUE EL PERSONAJE PUEDA ROTAR SOBRE SI MISMO CUANDO SUPERA UN ANGULO
    //
    // GetMesh()->SetWorldRotation(FRotator(GetMesh()->GetComponentRotation().Pitch,
    //                                      _PlayerCamera->GetComponentRotation().Yaw - 90.f,
    //                                      GetMesh()->GetComponentRotation().Roll));
    // 
    // if (bHeadTurn || bHeadTurn) {
    //     bHeadTurn = false;
    //     bHeadTurning = false;
    // }
}

/************** OVERLAPPING *************/
void AVRCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                             bool bFromSweep, const FHitResult& SweepResult) {

    if (SweepResult.Actor.IsValid()) {
        TArray<UActorComponent*> Components;
        OtherActor->GetComponents(Components);

        UStaticMeshComponent* _StaticMesh;
        bool HitItem = false;
        for (UActorComponent* Component : Components) {
            // Highlight outline colors:
            // GREEN: 252 | BLUE: 253 | ORANGE: 254 | WHITE: 255
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
            APlayerController* PlayerController = Cast<APlayerController>(GetController());
            if (OverlappedComponent == _LeftSphere) {
                _ActorFocusedLeft = OtherActor;
                if (PlayerController)
                    PlayerController->ClientPlayForceFeedback(_RumbleOverLapLeft, false, "rumble");
                SERVER_UpdateAnimation(EGripEnum::CanGrab, 1);
            }
            else if (OverlappedComponent == _RightSphere) {
                _ActorFocusedRight = OtherActor;
                if (PlayerController)
                    PlayerController->ClientPlayForceFeedback(_RumbleOverLapRight, false, "rumble");
                SERVER_UpdateAnimation(EGripEnum::CanGrab, 2);
            }
        }

        if (OtherActor == this){
            if (OverlappedComponent == _LeftSphere) {
                _ActorFocusedLeft = OtherActor;
                _ComponentFocusedLeft = OtherComp;
                UE_LOG(LogTemp, Warning, TEXT("%s"), *_ComponentFocusedLeft->GetFName().ToString());
            }
            else if (OverlappedComponent == _RightSphere) {
                _ActorFocusedRight = OtherActor;
                _ComponentFocusedRight = OtherComp;
                UE_LOG(LogTemp, Warning, TEXT("%s"), *_ComponentFocusedRight->GetFName().ToString());
            }
        }
    }
}

void AVRCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    if (OtherActor == _ActorFocusedLeft) {
        _ActorFocusedLeft = nullptr;
        _ComponentFocusedLeft = nullptr;
        SERVER_UpdateAnimation(EGripEnum::Open, 1);
    }
    else if (OtherActor == _ActorFocusedRight) {
        _ActorFocusedRight = nullptr;
        _ComponentFocusedRight = nullptr;
        SERVER_UpdateAnimation(EGripEnum::Open, 2);
    }

    UStaticMeshComponent* _StaticMesh = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));
    if (_StaticMesh) {
        _StaticMesh->SetCustomDepthStencilValue(0);
        _StaticMesh->SetRenderCustomDepth(false);
    }
}

/****************************************** ACTION MAPPINGS **************************************/
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
        else if (_ActorFocusedLeft) UseTriggerPressed(_ActorFocusedLeft, _SM_LeftHand, 1);
    }
    else _MenuInteractionComp->PressPointer();

    /* ANIMATION */
    SERVER_UpdateAnimation(EGripEnum::Grab, 1);
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
        else if (_ActorFocusedLeft || _ActorGrabbing)
            UseTriggerReleased(_ActorFocusedLeft, _SM_LeftHand, 1);
    }
    else _MenuInteractionComp->ReleasePointer();

    /* ANIMATION */
    SERVER_UpdateAnimation(EGripEnum::Open, 1);
}

/******* USE ITEM RIGHT *** ******/
void AVRCharacter::UseRightPressed(bool IsMenuHidden) {
    if (IsMenuHidden) {
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
        else if (_ActorFocusedRight) UseTriggerPressed(_ActorFocusedRight, _SM_RightHand, 2);
    }
    else _MenuInteractionComp->PressPointer();

    /* ANIMATION */
    SERVER_UpdateAnimation(EGripEnum::Grab, 2);
}

void AVRCharacter::UseRightReleased(bool IsMenuHidden) {
    if (IsMenuHidden) {
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
        else if (_ActorFocusedRight || _ActorGrabbing)
            UseTriggerReleased(_ActorFocusedRight, _SM_RightHand, 2);
    }
    else _MenuInteractionComp->ReleasePointer();

    /* ANIMATION */
    SERVER_UpdateAnimation(EGripEnum::Open, 2);
}

/*************** USE TRIGGER *************/
void AVRCharacter::UseTriggerPressed(AActor* ActorFocused, USceneComponent* InParent, int Hand) {
    UE_LOG(LogTemp, Warning, TEXT("Trigger actor: %s"), *ActorFocused->GetFName().ToString());
    if (ActorFocused) {
        /* CAN BE GRABBED */
        UGrabItem* GrabItemComp = Cast<UGrabItem>(ActorFocused->GetComponentByClass(
            UGrabItem::StaticClass()));
        if (GrabItemComp) {
            /* FOCUS */
            UStaticMeshComponent* _StaticMesh = Cast<UStaticMeshComponent>(ActorFocused->GetComponentByClass(
                UStaticMeshComponent::StaticClass()));
            if (_StaticMesh) {
                _StaticMesh->SetCustomDepthStencilValue(0);
                _StaticMesh->SetRenderCustomDepth(false);
            }
            SERVER_GrabPress(ActorFocused, InParent, FName("TakeSocket"), Hand);
        }
        else if (ActorFocused == this) {
            UE_LOG(LogTemp, Warning, TEXT("Yeaaaaaaaah"));
            if (_ComponentFocusedLeft) {
                if (_ComponentFocusedLeft->GetFName().ToString() == TEXT("PouchLeft") && _ActorPouchLeft) {
                    SERVER_GrabPress(_ActorPouchLeft, InParent, FName("TakeSocket"), Hand);
                    _ActorPouchLeft = nullptr;
                }
                else if (_ComponentFocusedLeft->GetFName().ToString() == TEXT("PouchRight") && _ActorPouchRight) {
                    SERVER_GrabPress(_ActorPouchRight, InParent, FName("TakeSocket"), Hand);
                    _ActorPouchRight = nullptr;
                }
            }
            else if (_ComponentFocusedRight) {
                if (_ComponentFocusedRight->GetFName().ToString() == TEXT("PouchLeft") && _ActorPouchLeft) {
                    SERVER_GrabPress(_ActorPouchLeft, InParent, FName("TakeSocket"), Hand);
                    _ActorPouchLeft = nullptr;
                }
                else if (_ComponentFocusedRight->GetFName().ToString() == TEXT("PouchRight") && _ActorPouchRight) {
                    SERVER_GrabPress(_ActorPouchRight, InParent, FName("TakeSocket"), Hand);
                    _ActorPouchRight = nullptr;
                }
            }
        }
        else {
            /* CAN BE USED */
            TArray<UActorComponent*> Components;
            ActorFocused->GetComponents(Components);
            for (UActorComponent* Component : Components) {
                if (Component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                    SERVER_UsePressed(Component);
                }
            }
        }
    }
}

void AVRCharacter::UseTriggerReleased(AActor* ActorFocused, USceneComponent* InParent, int Hand) {
    if (_ActorGrabbing) {
        /* DROP GRABBING */
        UGrabItem* GrabItemComp = Cast<UGrabItem>(_ActorGrabbing->GetComponentByClass(
            UGrabItem::StaticClass()));
        if (GrabItemComp) {
            /* FOCUS */
            UStaticMeshComponent* _StaticMesh = Cast<UStaticMeshComponent>(_ActorGrabbing->GetComponentByClass(
                UStaticMeshComponent::StaticClass()));
            if (_StaticMesh) {
                _StaticMesh->SetCustomDepthStencilValue(0);
                _StaticMesh->SetRenderCustomDepth(false);
            }
            /* Drop item */
            SERVER_GrabRelease(Hand);
        }
    }
    else if (ActorFocused) {
        /* CAN BE USED */
        TArray<UActorComponent*> Components;
        ActorFocused->GetComponents(Components);
        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                SERVER_UseReleased(Component);
            }
        }
    }
}

/********** TAKE ITEM ***********/
bool AVRCharacter::SERVER_GrabPress_Validate(AActor* Actor, USceneComponent* InParent,
                                             FName SocketName, int Hand) {
    return true;
}
void AVRCharacter::SERVER_GrabPress_Implementation(AActor* Actor, USceneComponent* InParent,
                                                   FName SocketName, int Hand) {
    MULTI_GrabPress(Actor, InParent, SocketName, Hand);
}
void AVRCharacter::MULTI_GrabPress_Implementation(AActor* Actor, USceneComponent* InParent,
                                                  FName SocketName, int Hand) {
    if (Actor) {
        _ActorGrabbing = Actor;
        UGrabItem* GrabItemComp = Cast<UGrabItem>(_ActorGrabbing->FindComponentByClass(
            UGrabItem::StaticClass()));
        if (GrabItemComp && InParent) {
            GrabItemComp->BeginGrab(InParent, SocketName);
            _ActorGrabbing->SetActorEnableCollision(false);

            if (Hand == 1) GrabItemComp->AddOnGrabDelegate(_GrabDelegateLeft);
            else if (Hand == 2) GrabItemComp->AddOnGrabDelegate(_GrabDelegateRight);
        }
    }
}

bool AVRCharacter::SERVER_GrabRelease_Validate(int Hand) {
    return true;
}
void AVRCharacter::SERVER_GrabRelease_Implementation(int Hand) {
    MULTI_GrabRelease(Hand);
}
void AVRCharacter::MULTI_GrabRelease_Implementation(int Hand) {
    if (_ActorGrabbing) {
        UGrabItem* GrabItemComp = Cast<UGrabItem>(_ActorGrabbing->FindComponentByClass(
            UGrabItem::StaticClass()));
        if (GrabItemComp) {
            GrabItemComp->EndGrab(true);
            _ActorGrabbing->SetActorEnableCollision(true);
            _ActorGrabbing = nullptr;
        }
    }
}

void AVRCharacter::ItemGrabbedLeft() {
    if (_ActorGrabbing) {
        CLIENT_AddRadioDelegates(_ActorGrabbing);

        UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(_ActorGrabbing->GetComponentByClass(
            UStaticMeshComponent::StaticClass()));
        UGrabItem* GrabItemComp = Cast<UGrabItem>(_ActorGrabbing->FindComponentByClass(
            UGrabItem::StaticClass()));
        if (ItemMesh && GrabItemComp) {
            ItemMesh->SetSimulatePhysics(false);
            ItemMesh->AttachToComponent(_SM_LeftHand,
                                        FAttachmentTransformRules::KeepRelativeTransform,
                                        TEXT("TakeSocket"));

            ItemMesh->RelativeLocation = GrabItemComp->_locationAttach_L;
            ItemMesh->RelativeRotation = GrabItemComp->_rotationAttach_L;

            _ActorGrabbing->SetActorEnableCollision(false);
            _ItemLeft = _ActorGrabbing;
            _ActorGrabbing = nullptr;
        }
    }
}

void AVRCharacter::ItemGrabbedRight() {
    if (_ActorGrabbing) {
        CLIENT_AddRadioDelegates(_ActorGrabbing);

        UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(_ActorGrabbing->GetComponentByClass(
            UStaticMeshComponent::StaticClass()));
        UGrabItem* GrabItemComp = Cast<UGrabItem>(_ActorGrabbing->FindComponentByClass(
            UGrabItem::StaticClass()));
        if (ItemMesh && GrabItemComp) {
            ItemMesh->SetSimulatePhysics(false);
            ItemMesh->AttachToComponent(_SM_RightHand,
                                        FAttachmentTransformRules::KeepRelativeTransform,
                                        TEXT("TakeSocket"));

            ItemMesh->RelativeLocation = GrabItemComp->_locationAttach_R;
            ItemMesh->RelativeRotation = GrabItemComp->_rotationAttach_R;

            _ActorGrabbing->SetActorEnableCollision(false);
            _ItemRight = _ActorGrabbing;
            _ActorGrabbing = nullptr;
        }
    }
}

/********** DROP HAND ***********/
void AVRCharacter::DropLeft() {
    if (_ItemLeft && _ItemLeft->GetComponentByClass(UGrabItem::StaticClass())) {
        SERVER_Drop(_ItemLeft, 1);
    }
}

void AVRCharacter::DropRight() {
    if (_ItemRight && _ItemRight->GetComponentByClass(UGrabItem::StaticClass())) {
        SERVER_Drop(_ItemRight, 2);
    }
}

bool AVRCharacter::SERVER_Drop_Validate(AActor* ItemActor, int Hand) { return true; }
void AVRCharacter::SERVER_Drop_Implementation(AActor* ItemActor, int Hand) {
    MULTI_Drop(ItemActor, Hand);
}
void AVRCharacter::MULTI_Drop_Implementation(AActor* ItemActor, int Hand) {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ItemActor->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));
    if (ItemMesh && (_ComponentFocusedLeft || _ComponentFocusedRight)) {
        if (_ComponentFocusedLeft) {
            FString _CompName = _ComponentFocusedLeft->GetFName().ToString();

            if (_CompName == TEXT("PouchLeft") && _ActorPouchLeft == nullptr) {
                ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
                ItemActor->SetActorEnableCollision(true);
                ItemMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Pouch1"));
                _ActorPouchLeft = Cast<AActor>(ItemMesh);
            }
            else if (_CompName == TEXT("PouchRight") && _ActorPouchRight == nullptr) {
                ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
                ItemActor->SetActorEnableCollision(true);
                ItemMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Pouch2"));
                _ActorPouchRight = Cast<AActor>(ItemMesh);
            }

            if (Hand == 1) _ItemLeft = nullptr;
            else if (Hand == 2) _ItemRight = nullptr;
        }
        if (_ComponentFocusedRight) {
            FString _CompName = _ComponentFocusedRight->GetFName().ToString();

            if (_CompName == TEXT("PouchLeft") && _ActorPouchLeft == nullptr) {
                ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
                ItemActor->SetActorEnableCollision(true);
                ItemMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Pouch1"));
                _ActorPouchLeft = Cast<AActor>(ItemMesh);
            }
            else if (_CompName == TEXT("PouchRight") && _ActorPouchRight == nullptr) {
                ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
                ItemActor->SetActorEnableCollision(true);
                ItemMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Pouch2"));
                _ActorPouchRight = Cast<AActor>(ItemMesh);
            }

            if (Hand == 1) _ItemLeft = nullptr;
            else if (Hand == 2) _ItemRight = nullptr;
        }
    }
    else if (ItemMesh) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        ItemMesh->SetSimulatePhysics(true);

        ItemActor->SetActorEnableCollision(true);

        if (Hand == 1) _ItemLeft = nullptr;
        else if (Hand == 2) _ItemRight = nullptr;
    }
}

/************ VR CHARACTER IK FEATURES *************/
void AVRCharacter::UpdateIK() {
    _HMDWorldPosition = _PlayerCamera->GetComponentLocation() + HeadCameraOffset;
    _HMDWorldOrientation = _PlayerCamera->GetComponentRotation();
    _LeftControllerPosition = _LeftHandComp->GetComponentLocation();
    _LeftControllerOrientation = _SM_LeftHand->GetComponentRotation();
    _RightControllerPosition = _RightHandComp->GetComponentLocation();
    _RightControllerOrientation = _SM_RightHand->GetComponentRotation();
}

/************ VR CHARACTER CALIBRATION FEATURES *************/

void AVRCharacter::CalculateMeshArmExtension() {
    if (GetMesh() != nullptr) {
        FVector Hand = GetMesh()->GetBoneLocation(TEXT("hand_l"));
        FVector Arm = GetMesh()->GetBoneLocation(TEXT("lowerarm_l"));
        FVector Shoulder = GetMesh()->GetBoneLocation(TEXT("upperarm_l"));
        MaxMeshArmExtension = (Arm - Hand).Size() + (Shoulder - Arm).Size();
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Longitud de brazo no calculado. No existe malla de personaje."))
    }
}

/********** UPDATE ANIMATIONS ***********/
bool AVRCharacter::SERVER_UpdateAnimation_Validate(EGripEnum NewAnim, int Hand) {
    return true;
}
void AVRCharacter::SERVER_UpdateAnimation_Implementation(EGripEnum NewAnim, int Hand) {
    MULTI_UpdateAnimation(NewAnim, Hand);
}
void AVRCharacter::MULTI_UpdateAnimation_Implementation(EGripEnum NewAnim, int Hand) {
    if (Hand == 1) {
        _GripStateLeft = NewAnim;
    }
    else if (Hand == 2) {
        _GripStateRight = NewAnim;
    }
}
