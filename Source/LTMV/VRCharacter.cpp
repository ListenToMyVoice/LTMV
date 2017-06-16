// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "VRCharacter.h"

#include "ItfUsable.h"
#include "ItfUsableItem.h"
#include "GrabItem.h"
#include "Token.h"
#include "TokenHolder.h"
#include "NWGameInstance.h"

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
    
    GetCapsuleComponent()->SetCapsuleRadius(32.f);

    _VROriginComp = CreateDefaultSubobject<USceneComponent>(TEXT("_VROriginComp"));
    _VROriginComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    _VROriginComp->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
    
    GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
    GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetOnlyOwnerSee(false);

    _PlayerCamera->AttachToComponent(_VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);
    _PlayerCamera->PostProcessBlendWeight = 1;
    _ChaperoneComp = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("_ChaperoneComp"));

    _PouchLeft = CreateDefaultSubobject<USphereComponent>(TEXT("PouchLeft"));
    _PouchLeft->SetSphereRadius(5.0f);
    _PouchRight = CreateDefaultSubobject<USphereComponent>(TEXT("PouchRight"));
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
	_LeftHandComp->SetOnlyOwnerSee(true);
	_LeftHandComp->SetOwnerNoSee(false);

    /* MESH */
    _SM_LeftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("_SM_LeftHand"));
    _SM_LeftHand->AttachToComponent(_LeftHandComp, FAttachmentTransformRules::KeepRelativeTransform);
    _SM_LeftHand->SetWorldScale3D(FVector(1.0f, 1.0f, -1.0f));
    _SM_LeftHand->SetRelativeRotation(FRotator(0.f, 0.0f, 90.f));
    _SM_LeftHand->SetRelativeLocation(FVector(-10.f, 0.f, 0.f));
	_SM_LeftHand->SetOnlyOwnerSee(true);
	_SM_LeftHand->SetOwnerNoSee(false);

    /* ADDITIONAL */
    _LeftSphere = CreateDefaultSubobject<USphereComponent>(TEXT("_LeftSphere"));
    _LeftSphere->AttachToComponent(_SM_LeftHand, FAttachmentTransformRules::KeepRelativeTransform);
    _LeftSphere->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
    _LeftSphere->SetSphereRadius(14.f);
	_LeftSphere->SetOnlyOwnerSee(true);
	_LeftSphere->SetOwnerNoSee(false);
	_LeftSphere->SetHiddenInGame(true);
}

void AVRCharacter::BuildRight() {
    _RightHandComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("_RightHandComp"));
    _RightHandComp->Hand = EControllerHand::Right;
    _RightHandComp->AttachToComponent(_VROriginComp, FAttachmentTransformRules::KeepRelativeTransform);
    _RightHandComp->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
	_RightHandComp->SetOnlyOwnerSee(true);
	_RightHandComp->SetOwnerNoSee(false);

    /* MESH */
    _SM_RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("_SM_RightHand"));
    _SM_RightHand->AttachToComponent(_RightHandComp, FAttachmentTransformRules::KeepRelativeTransform);
    _SM_RightHand->SetRelativeRotation(FRotator(0.f, 0.0f, 90.f));
    _SM_RightHand->SetRelativeLocation(FVector(-10.f, 0.f, 0.f));
	_SM_RightHand->SetOnlyOwnerSee(true);
	_SM_RightHand->SetOwnerNoSee(false);

    /* ADDITIONAL */
    _RightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("_RightSphere"));
    _RightSphere->AttachToComponent(_SM_RightHand, FAttachmentTransformRules::KeepRelativeTransform);
    _RightSphere->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
    _RightSphere->SetSphereRadius(14.f);
	_RightSphere->SetOnlyOwnerSee(true);
	_RightSphere->SetOwnerNoSee(false);
	_RightSphere->SetHiddenInGame(true);
}

void AVRCharacter::BeginPlay() {
    Super::BeginPlay();

    HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());
    if (HMD) HMD->EnablePositionalTracking(bPositionalHeadTracking);

    if (IsPlayerControlled()) {
        APlayerController* const PC = Cast<APlayerController>(GetController());
        PC->InputYawScale = 1.0f;
    }

    bHeadTurn = false;
    bHeadTurning = false;

    _GrabDelegateLeft.BindUObject(this, &AVRCharacter::ItemGrabbedLeft);
    _GrabDelegateRight.BindUObject(this, &AVRCharacter::ItemGrabbedRight);

	if (!GI) {
		GI = Cast<UNWGameInstance>(GetGameInstance());
	}

    //CalculateMeshArmExtension();
}

void AVRCharacter::AfterPossessed(bool SetInventory) {
	if (SetInventory) {
		// Poner el tutorial del bunker
	}
	else {
		// tutorial de la cueva
	}
}

void AVRCharacter::Tick(float deltaTime) {
    Super::Tick(deltaTime);

	UpdateMeshPostitionWithCamera();
	UpdateMeshRotationWithCamera();

    UpdateIK();
    
    SERVER_UpdateComponentPosition(_LeftHandComp, _LeftHandComp->RelativeLocation,
                                                  _LeftHandComp->RelativeRotation);

    SERVER_UpdateComponentPosition(_RightHandComp, _RightHandComp->RelativeLocation,
                                                   _RightHandComp->RelativeRotation);

}

void AVRCharacter::UpdateMeshPostitionWithCamera() {
	GetMesh()->SetWorldLocation(_PlayerCamera->GetComponentLocation() -
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
	float CameraYaw = _PlayerCamera->GetComponentRotation().Yaw;
    float CharacterYaw = this->GetActorRotation().Yaw;
    float RelativeYaw = CameraYaw - CharacterYaw;

    if (RelativeYaw >= MaxHeadTurnValue || RelativeYaw <= -MaxHeadTurnValue) {
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

    GetMesh()->SetWorldRotation(NextRotation);

    if ((NextRotation - TargetRotation).IsNearlyZero()) { bHeadTurning = false; }
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
    PlayerInput->BindAxis("TurnAtRate", this, &AVRCharacter::TurnAtRate);
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
    _PlayerCamera->PostProcessSettings.bOverride_VignetteIntensity = true;

	if (GI && GI->_MenuOptions.bComfortMode && Value != 0) {
		_PlayerCamera->PostProcessSettings.VignetteIntensity = 2;
		float ActorYaw = GetActorRotation().Yaw;
		float MeshYaw = GetMesh()->GetComponentRotation().Yaw + 90.f;

		GetMesh()->SetWorldRotation(FRotator(GetMesh()->GetComponentRotation().Pitch,
											 ActorYaw - MeshYaw,
											 GetMesh()->GetComponentRotation().Roll));

		AddMovementInput(GetActorForwardVector(), Value);
	}
	else {
		_PlayerCamera->PostProcessSettings.VignetteIntensity = 0;
		float CameraYaw = _PlayerCamera->GetComponentRotation().Yaw;
		float MeshYaw = GetMesh()->GetComponentRotation().Yaw + 90.f;

		GetMesh()->SetWorldRotation(FRotator(GetMesh()->GetComponentRotation().Pitch,
									CameraYaw - MeshYaw,
									GetMesh()->GetComponentRotation().Roll));

		AddMovementInput(FVector(_PlayerCamera->GetForwardVector().X, _PlayerCamera->GetForwardVector().Y, 0), Value);
	}
}

void AVRCharacter::TurnAtRate(float Rate) {
	if (GI && GI->_MenuOptions.bComfortMode) {
		AddControllerYawInput(Rate * 15.f);
		SetActorRotation(FRotator(GetActorRotation().Pitch, Rate * 15.f, GetActorRotation().Roll));
		HMD->ResetOrientation();
	}
}

//void AVRCharacter::TurnVRCharacter() {
//    float _CameraYawValue = _PlayerCamera->GetComponentRotation().Yaw;
//    float _PlayerYawValue = GetActorRotation().Yaw;
//    float _MeshYawValue = GetMesh()->GetComponentRotation().Yaw + 90.f;
//
//    float _YawRelativeValue = _CameraYawValue - _PlayerYawValue;
//    float _MeshYawRelativeValue = _CameraYawValue - _MeshYawValue;
//
//    AddControllerYawInput(_YawRelativeValue);
//    SetActorRotation(FRotator(GetActorRotation().Pitch, _YawRelativeValue, GetActorRotation().Roll));
//
//    HMD->ResetOrientation();
//}

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

				// tutorial como coger objetos con el Trigger!!!

                if (_StaticMesh) {
                    _StaticMesh->SetRenderCustomDepth(true);
                    _StaticMesh->SetCustomDepthStencilValue(255);
                    HitItem = true;
                }
            }
			else if (Component->GetClass() == UTokenHolder::StaticClass()) {
				_StaticMesh = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(
					UStaticMeshComponent::StaticClass()));

				// tutorial como coger objetos con el Trigger!!!

				if (_StaticMesh) {
					_StaticMesh->SetRenderCustomDepth(true);
					_StaticMesh->SetCustomDepthStencilValue(254);
					HitItem = true;
				}
			}
        }

        if (HitItem) {
            APlayerController* PlayerController = Cast<APlayerController>(GetController());
            if (OverlappedComponent == _LeftSphere) {
                _ActorFocusedLeft = OtherActor;
				_ComponentFocusedLeft = OtherComp;
                if (PlayerController)
                    PlayerController->ClientPlayForceFeedback(_RumbleOverLapLeft, false, "rumble");
                SERVER_UpdateAnimation(EGripEnum::CanGrab, 1);
            }
            else if (OverlappedComponent == _RightSphere) {
                _ActorFocusedRight = OtherActor;
				_ComponentFocusedRight = OtherComp;
                if (PlayerController)
                    PlayerController->ClientPlayForceFeedback(_RumbleOverLapRight, false, "rumble");
                SERVER_UpdateAnimation(EGripEnum::CanGrab, 2);
            }
        }

        if (OtherActor == this){
            if (OverlappedComponent == _LeftSphere) {
                _ActorFocusedLeft = OtherActor;
                _ComponentFocusedLeft = OtherComp;

                if(_ComponentFocusedLeft == _PouchLeft && _ActorPouchLeft != nullptr) {
                    _ActorFocusedLeft = _ActorPouchLeft;
                }
                else if (_ComponentFocusedLeft == _PouchRight && _ActorPouchRight != nullptr) {
                    _ActorFocusedLeft = _ActorPouchRight;
                }
            }
            else if (OverlappedComponent == _RightSphere) {
                _ActorFocusedRight = OtherActor;
                _ComponentFocusedRight = OtherComp;

                if (_ComponentFocusedRight == _PouchLeft && _ActorPouchLeft != nullptr) {
                    _ActorFocusedRight = _ActorPouchLeft;
                }
                else if (_ComponentFocusedRight == _PouchRight && _ActorPouchRight != nullptr) {
                    _ActorFocusedRight = _ActorPouchRight;
                }
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
    if (ActorFocused) {
        /* CAN BE GRABBED */
        UGrabItem* GrabItemComp = Cast<UGrabItem>(ActorFocused->GetComponentByClass(
            UGrabItem::StaticClass()));
        if (GrabItemComp) {
            /* FOCUS */

			// tutorial de como utilizar el objeto.

            UStaticMeshComponent* _StaticMesh = Cast<UStaticMeshComponent>(ActorFocused->GetComponentByClass(
                UStaticMeshComponent::StaticClass()));
            if (_StaticMesh) {
                _StaticMesh->SetCustomDepthStencilValue(0);
                _StaticMesh->SetRenderCustomDepth(false);
            }

            // TODO: BUG AL COGER DOS OBJETOS A LA VEZ. Si descomentamos la linea a continuacion
            // y comentamos los 3 if/elseif/else siguientes, compilamos, iniciamos e intentamos agarrar dos objetos a la vez
            // uno con cada mano, hay un bug en el que uno de los objetos se queda a mitad del trayecto.
            //SERVER_GrabPress(ActorFocused, InParent, FName("TakeSocket"), Hand);

            if (_ActorPouchLeft != nullptr && ActorFocused == _ActorPouchLeft) {
                _StaticMesh->SetMobility(EComponentMobility::Movable);
                _StaticMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
                _StaticMesh->SetSimulatePhysics(true);
                _ActorPouchLeft->SetActorEnableCollision(true);
                
                SERVER_GrabPress(ActorFocused, InParent, FName("TakeSocket"), Hand);
                _ActorPouchLeft = nullptr;
            }
            else if (_ActorPouchRight != nullptr && ActorFocused == _ActorPouchRight) {
                _StaticMesh->SetMobility(EComponentMobility::Movable);
                _StaticMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
                _StaticMesh->SetSimulatePhysics(true);
                _ActorPouchRight->SetActorEnableCollision(true);
                
                SERVER_GrabPress(ActorFocused, InParent, FName("TakeSocket"), Hand);
                _ActorPouchRight = nullptr;
            }
            else {
                SERVER_GrabPress(ActorFocused, InParent, FName("TakeSocket"), Hand);
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

			// tutorial de como dropear a inventario o a mundo.

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

	if (ItemMesh && _ActorFocusedLeft && _ComponentFocusedLeft &&
		_ActorFocusedLeft->GetComponentByClass(UTokenHolder::StaticClass()) &&
		ItemActor->GetComponentByClass(UToken::StaticClass())) {

		UTokenHolder* Holder = Cast<UTokenHolder>(_ActorFocusedLeft->GetComponentByClass(UTokenHolder::StaticClass()));

		ItemMesh->SetMobility(EComponentMobility::Movable);
		ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		
		ItemMesh->AttachToComponent(Cast<USceneComponent>(_ActorFocusedLeft),
									FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("TablillaSocket"));

		Holder->_Tablilla = ItemActor;
	}

	else if (ItemMesh && _ActorFocusedRight && _ComponentFocusedRight &&
		_ActorFocusedRight->GetComponentByClass(UTokenHolder::StaticClass())) {
	
		UTokenHolder* Holder = Cast<UTokenHolder>(_ActorFocusedRight->GetComponentByClass(UTokenHolder::StaticClass()));

		ItemMesh->SetMobility(EComponentMobility::Movable);
		ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		ItemMesh->AttachToComponent(Cast<USceneComponent>(_ActorFocusedRight),
									FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("TablillaSocket"));

		Holder->_Tablilla = ItemActor;		
	}

    // DROP TO INVENTORY POUCH
    else if ((ItemMesh && _ComponentFocusedLeft && Hand == 1) || (ItemMesh && _ComponentFocusedRight && Hand == 2)) {

        switch (Hand) {
        case 1:
            if (_ComponentFocusedLeft == _PouchLeft && _ActorPouchLeft == nullptr) {

                ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
                ItemMesh->AttachToComponent(_PouchLeft, FAttachmentTransformRules::KeepRelativeTransform);

                ItemActor->SetActorEnableCollision(true);
                _ActorPouchLeft = ItemActor;
            }

            else if (_ComponentFocusedLeft == _PouchRight && _ActorPouchRight == nullptr) {

                ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
                ItemMesh->AttachToComponent(_PouchRight, FAttachmentTransformRules::KeepRelativeTransform);

                ItemActor->SetActorEnableCollision(true);
                _ActorPouchRight = ItemActor;
            }
            break;
        case 2:
            if (_ComponentFocusedRight == _PouchLeft && _ActorPouchLeft == nullptr) {

                ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
				ItemMesh->AttachToComponent(_PouchLeft, FAttachmentTransformRules::KeepRelativeTransform);

                ItemActor->SetActorEnableCollision(true);
                _ActorPouchLeft = ItemActor;
            }

            else if (_ComponentFocusedRight == _PouchRight && _ActorPouchRight == nullptr) {

                ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
				ItemMesh->AttachToComponent(_PouchRight, FAttachmentTransformRules::KeepRelativeTransform);

                ItemActor->SetActorEnableCollision(true);
                _ActorPouchRight = ItemActor;
            }
            break;
        default:
            break;
        }
    }

    // DROP TO FLOOR
    else if (ItemMesh) {

        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        ItemMesh->SetSimulatePhysics(true);

        ItemActor->SetActorEnableCollision(true);
    }

    if (Hand == 1) _ItemLeft = nullptr;
    else if (Hand == 2) _ItemRight = nullptr;
}

/************ VR CHARACTER IK FEATURES *************/
void AVRCharacter::UpdateIK() {
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

/* OTHER SERVER FUNCTIONS */
/********** UPDATE SERVER CONTROLLER LOCATIONS ***********/
bool AVRCharacter::SERVER_UpdateComponentPosition_Validate(USceneComponent* Component,
	FVector Location, FRotator Rotation) {
	return true;
}
void AVRCharacter::SERVER_UpdateComponentPosition_Implementation(USceneComponent* Component,
	FVector Location, FRotator Rotation) {
	MULTI_UpdateComponentPosition(Component, Location, Rotation);
}
void AVRCharacter::MULTI_UpdateComponentPosition_Implementation(USceneComponent* Component,
	FVector Location, FRotator Rotation) {
	Component->SetRelativeLocationAndRotation(Location, Rotation);
}

/********** UPDATE SERVER ANIMATIONS ***********/
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

/******** AUXILIARY FUNCTIONS ********/

AActor* AVRCharacter::GetActorFocusedLeft() { return _ActorFocusedLeft; }
AActor* AVRCharacter::GetActorFocusedRight() { return _ActorFocusedRight; }