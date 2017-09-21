// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "VRCharacter.h"

#include "Inventory.h"
#include "InventoryItem.h"
#include "VRInventory.h"

#include "ItfUsable.h"
#include "ItfUsableItem.h"
#include "HandPickItem.h"
#include "GrabItem.h"
#include "Token.h"
#include "TokenHolder.h"
#include "NWGameInstance.h"
#include "MenuInteraction.h"
#include "WidgetComponent.h"
#include "VRInventory.h"

#include "PlayerControllerPlay.h"

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
	_PlayerCamera->bLockToHmd = true;
    _PlayerCamera->PostProcessBlendWeight = 1;
    _ChaperoneComp = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("_ChaperoneComp"));
    _ChaperoneComp->Activate();

    _Inventory = CreateDefaultSubobject<UInventory>(TEXT("VR Inventory"));
    _Inventory->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("inventory"));

    HMD = nullptr;
    this->BaseEyeHeight = 0.f;
    this->CrouchedEyeHeight = 0.f;
    _GripStateLeft = EGripEnum::Open;
    _GripStateRight = EGripEnum::Open;
    MaxHeadTurnValue = 80.f;

    /* LEFT HAND BUILD START */
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

    /* FOR OVERLAP INTERACTION */
    _LeftSphere = CreateDefaultSubobject<USphereComponent>(TEXT("_LeftSphere"));
    _LeftSphere->AttachToComponent(_SM_LeftHand, FAttachmentTransformRules::KeepRelativeTransform);
    _LeftSphere->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
    _LeftSphere->SetSphereRadius(14.f);
    _LeftSphere->SetOnlyOwnerSee(true);
    _LeftSphere->SetOwnerNoSee(false);
    _LeftSphere->SetHiddenInGame(true);

    /* FOR WIDGET INTERACTION */
    _LeftInteractor = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("LeftInteractor"));
    _LeftInteractor->AttachToComponent(_SM_LeftHand, FAttachmentTransformRules::KeepRelativeTransform);
    _LeftInteractor->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
    _LeftInteractor->SetComponentTickEnabled(false);
    _LeftInteractor->InteractionDistance = 1000.f;
    _LeftInteractor->SetIsReplicated(false);
    _LeftInteractor->SetHiddenInGame(true);
    _LeftInteractor->SetVisibility(true);
    _LeftInteractor->SetActive(false);

    /* LEFT SPLINE */
    _LeftSpline = CreateDefaultSubobject<USplineMeshComponent>(TEXT("Left Spline"));
    _LeftSpline->AttachToComponent(_SM_LeftHand, FAttachmentTransformRules::KeepRelativeTransform);
    _LeftSpline->SetRelativeLocation(FVector(10.f, 0.f, 0.f));

    _LeftSphere->OnComponentBeginOverlap.AddDynamic(this, &AVRCharacter::OnOverlap);
    _LeftSphere->OnComponentEndOverlap.AddDynamic(this, &AVRCharacter::OnEndOverlap);
    /* LEFT HAND BUILD END */

    /* RIGHT HAND BUILD START */
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

    /* FOR OVERLAP INTERACTION */
    _RightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("_RightSphere"));
    _RightSphere->AttachToComponent(_SM_RightHand, FAttachmentTransformRules::KeepRelativeTransform);
    _RightSphere->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
    _RightSphere->SetSphereRadius(14.f);
    _RightSphere->SetOnlyOwnerSee(true);
    _RightSphere->SetOwnerNoSee(false);
    _RightSphere->SetHiddenInGame(true);

    /* FOR WIDGET INTERACTION */
    _RightInteractor = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("RightInteractor"));
    _RightInteractor->AttachToComponent(_SM_RightHand, FAttachmentTransformRules::KeepRelativeTransform);
    _RightInteractor->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
    _RightInteractor->SetComponentTickEnabled(false);
    _RightInteractor->InteractionDistance = 1000.f;
    _RightInteractor->SetIsReplicated(false);
    _RightInteractor->SetHiddenInGame(true);
    _RightInteractor->SetVisibility(true);
    _RightInteractor->SetActive(false);

    /* RIGHT SPLINE */
    _RightSpline = CreateDefaultSubobject<USplineMeshComponent>(TEXT("Right Spline"));
    _RightSpline->AttachToComponent(_SM_RightHand, FAttachmentTransformRules::KeepRelativeTransform);
    _RightSpline->SetRelativeLocation(FVector(10.f, 0.f, 0.f));

    _RightSphere->OnComponentBeginOverlap.AddDynamic(this, &AVRCharacter::OnOverlap);
    _RightSphere->OnComponentEndOverlap.AddDynamic(this, &AVRCharacter::OnEndOverlap);
    /* RIGHT HAND BUILD END */

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

	_LastMeshPosition = GetMesh()->GetComponentLocation();

	// El character empieza con la pantalla en negro salvo en el menú principal.
	if (GetWorld()->GetCurrentLevel()->GetFName() != TEXT("MapMenu")) {
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC) {
			APlayerCameraManager* _CameraManager = PC->PlayerCameraManager;
			if (_CameraManager) {
				_CameraManager->bEnableFading = true;
				_CameraManager->FadeColor = FColor::Black;
				_CameraManager->FadeAmount = 1.f;
				bToBlack = true;
				FadeDisplay();
			}
		}
	}
}

void AVRCharacter::AfterPossessed(bool SetInventory, bool respawning) {
	if (SetInventory) {
		// Poner el tutorial del bunker
	}
	else {
		// tutorial de la cueva
	}

	// El character empieza con la pantalla en negro salvo en el menú principal.
	if (GetWorld()->GetCurrentLevel()->GetFName() != TEXT("MapMenu")) {
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC) {
			APlayerCameraManager* _CameraManager = PC->PlayerCameraManager;
			if (_CameraManager) {
				_CameraManager->bEnableFading = true;
				_CameraManager->FadeColor = FColor::Black;
				_CameraManager->FadeAmount = 1.f;
				bToBlack = true;
				FadeDisplay();
			}
		}
	}

    APlayerControllerPlay* PCP = Cast<APlayerControllerPlay>(GetController());
    if (PCP) bInventoryActive = !PCP->GetVRInventory()->bIsVRInventoryHidden;
}

void AVRCharacter::Tick(float deltaTime) {
    Super::Tick(deltaTime);

	UpdateIK();

	SERVER_UpdateMeshWithCamera();
   
    SERVER_UpdateComponentPosition(_LeftHandComp, _LeftHandComp->RelativeLocation,
                                                  _LeftHandComp->RelativeRotation);

    SERVER_UpdateComponentPosition(_RightHandComp, _RightHandComp->RelativeLocation,
                                                   _RightHandComp->RelativeRotation);

    UpdateWidgetLeftBeam();
    UpdateWidgetRightBeam();
}

void AVRCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInput) {
    Super::SetupPlayerInputComponent(PlayerInput);

    /* ACTIONS */
    PlayerInput->BindAction("DropLeft", IE_Pressed, this, &AVRCharacter::DropLeft);
    PlayerInput->BindAction("DropRight", IE_Pressed, this, &AVRCharacter::DropRight);

    /* VR SPECIFIC */
    PlayerInput->BindAction("ResetHMDOrigin", IE_Pressed, this, &AVRCharacter::ResetHMDOrigin);

    /* MOVEMENT */
	PlayerInput->BindAction("TurnLeftComfort", IE_Pressed, this, &AVRCharacter::TurnLeftComfort);
	PlayerInput->BindAction("TurnRightComfort", IE_Pressed, this, &AVRCharacter::TurnRightComfort);

	/* FADE CAMERA */
	PlayerInput->BindAction("FadeDisplay", IE_Pressed, this, &AVRCharacter::FadeDisplay);
}

void AVRCharacter::ResetHMDOrigin() {// R
    if (HMD) HMD->ResetOrientationAndPosition();
}

void AVRCharacter::FadeDisplay() {// T
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC) {
		APlayerCameraManager* _CameraManager = PC->PlayerCameraManager;
		if (_CameraManager && !bToBlack) {
			_CameraManager->StartCameraFade(0.f, 1.f, 5.f, FColor::Black, false, true);
			bToBlack = true;
		}

		else if (_CameraManager && bToBlack) {
			_CameraManager->StartCameraFade(1.f, 0.f, 10.f, FColor::Black, false, true);
			bToBlack = false;
		}
	}
}

void AVRCharacter::MoveForward(float Value) {
	float CameraYaw = _PlayerCamera->GetComponentRotation().Yaw - 90.f;

	if (Value > 0) {
		GetMesh()->SetWorldRotation(FRotator(GetMesh()->GetComponentRotation().Pitch,
											 CameraYaw,
											 GetMesh()->GetComponentRotation().Roll));
	}
	else if (Value < 0) {
		GetMesh()->SetWorldRotation(FRotator(GetMesh()->GetComponentRotation().Pitch,
											 CameraYaw,
											 GetMesh()->GetComponentRotation().Roll));
	}

	AddMovementInput(FVector(_PlayerCamera->GetForwardVector().X, _PlayerCamera->GetForwardVector().Y, 0.f), Value);
	CheckFloorMaterial();
}

// Funciones input que probablemente haya que replicar también.
void AVRCharacter::TurnLeftComfort() {
    UNWGameInstance* GInstance = Cast<UNWGameInstance>(GetGameInstance());
	if (GInstance && GInstance->_MenuOptions.bComfortMode) {
		SetActorRotation(FRotator(GetActorRotation().Pitch,
								  GetActorRotation().Yaw - 15.f,
								  GetActorRotation().Roll));

		AddControllerYawInput(-15.f);
	}
}
void AVRCharacter::TurnRightComfort() {
    UNWGameInstance* GInstance = Cast<UNWGameInstance>(GetGameInstance());
	if (GInstance && GInstance->_MenuOptions.bComfortMode) {
		SetActorRotation(FRotator(GetActorRotation().Pitch,
								  GetActorRotation().Yaw + 15.f,
								  GetActorRotation().Roll));

		AddControllerYawInput(+15.f);
	}
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
                if (PlayerController) PlayerController->ClientPlayForceFeedback(_RumbleOverLapLeft, false, "rumble");
                if (!_ItemLeft) SERVER_UpdateAnimation(EGripEnum::CanGrab, 1);
            }
            else if (OverlappedComponent == _RightSphere) {
                _ActorFocusedRight = OtherActor;
                if (PlayerController) PlayerController->ClientPlayForceFeedback(_RumbleOverLapRight, false, "rumble");
				if (!_ItemRight) SERVER_UpdateAnimation(EGripEnum::CanGrab, 2);
            }
        }
    }
}

void AVRCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    if (OtherActor == _ActorFocusedLeft) {
        _ActorFocusedLeft = nullptr;
        if (!_ItemLeft) SERVER_UpdateAnimation(EGripEnum::Open, 1);
    }
    else if (OtherActor == _ActorFocusedRight) {
        _ActorFocusedRight = nullptr;
		if (!_ItemRight) SERVER_UpdateAnimation(EGripEnum::Open, 2);
    }

    UStaticMeshComponent* _StaticMesh = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));
    if (_StaticMesh) {
        _StaticMesh->SetCustomDepthStencilValue(0);
        _StaticMesh->SetRenderCustomDepth(false);
    }
}

/************************** UPDATE MESH POSITION AND ROTATIONS WITH HMD **************************/
void AVRCharacter::UpdateMeshPostitionWithCamera() {

	GetMesh()->SetWorldLocation(FVector(_PlayerCamera->GetComponentLocation().X,
		_PlayerCamera->GetComponentLocation().Y,
		GetMesh()->GetComponentLocation().Z));

	_MeshSpeed = (GetMesh()->GetComponentLocation() - _LastMeshPosition).Size() / GetWorld()->GetDeltaSeconds();

	if (_MeshSpeed > GetCharacterMovement()->GetMaxSpeed()) {
		_MeshSpeed = GetCharacterMovement()->GetMaxSpeed();
	}

	_LastMeshPosition = GetMesh()->GetComponentLocation();
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

// Support functions CheckHeadTurn and TurnBody.
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

bool AVRCharacter::SERVER_UpdateMeshWithCamera_Validate() { return true; }
void AVRCharacter::SERVER_UpdateMeshWithCamera_Implementation() { MULTI_UpdateMeshWithCamera(); }
void AVRCharacter::MULTI_UpdateMeshWithCamera_Implementation() { 
	UpdateMeshPostitionWithCamera(); 
	UpdateMeshRotationWithCamera();
}

/****************************************** ACTION MAPPINGS **************************************/
/******** USE ITEM LEFT *********/
void AVRCharacter::UseLeftPressed(bool IsMenuHidden) {
    if (IsMenuHidden && !bInventoryActive) {
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
    else if (!IsMenuHidden) _MenuInteractionComp->PressPointer();
    else if (bInventoryActive) _LeftInteractor->PressPointerKey(EKeys::LeftMouseButton);

    /* ANIMATION */
    if (!_ItemLeft && !_ActorGrabbing) SERVER_UpdateAnimation(EGripEnum::Grab, 1);
}

void AVRCharacter::UseLeftReleased(bool IsMenuHidden) {
    if (IsMenuHidden && !bInventoryActive) {
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
        else if (_ActorFocusedLeft || _ActorGrabbing) UseTriggerReleased(_ActorFocusedLeft, _SM_LeftHand, 1);
    }
    else if (!IsMenuHidden) _MenuInteractionComp->PressPointer();
    else if (bInventoryActive) _LeftInteractor->ReleasePointerKey(EKeys::LeftMouseButton);

    /* ANIMATION */
    if (!_ItemLeft && !_ActorGrabbing) SERVER_UpdateAnimation(EGripEnum::Open, 1);
}

/******* USE ITEM RIGHT *** ******/
void AVRCharacter::UseRightPressed(bool IsMenuHidden) {
    if (IsMenuHidden && !bInventoryActive) {
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
    else if (!IsMenuHidden) _MenuInteractionComp->PressPointer();
    else if (bInventoryActive) _RightInteractor->PressPointerKey(EKeys::RightMouseButton);

    /* ANIMATION */
    if (!_ItemRight && !_ActorGrabbing) SERVER_UpdateAnimation(EGripEnum::Grab, 2);
}

void AVRCharacter::UseRightReleased(bool IsMenuHidden) {
    if (IsMenuHidden && !bInventoryActive) {
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
        else if (_ActorFocusedRight || _ActorGrabbing) UseTriggerReleased(_ActorFocusedRight, _SM_RightHand, 2);
    }
    else if (!IsMenuHidden) _MenuInteractionComp->ReleasePointer();
    else if (bInventoryActive) _RightInteractor->ReleasePointerKey(EKeys::RightMouseButton);

    /* ANIMATION */
    if (!_ItemRight && !_ActorGrabbing) SERVER_UpdateAnimation(EGripEnum::Open, 2);
}

/*************** USE TRIGGER *************/
void AVRCharacter::UseTriggerPressed(AActor* ActorFocused, USceneComponent* InParent, int Hand) {
    if (ActorFocused) {
        _LastActorFocused = ActorFocused;
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

            // TODO: BUG AL COGER DOS OBJETOS A LA VEZ. La linea a continuacion provoca que al iniciar
            // e intentar agarrar dos objetos a la vez uno con cada mano, ocurre un bug en el que uno
            // de los objetos se queda a mitad del trayecto.
            SERVER_GrabPress(ActorFocused, InParent, FName("TakeSocket"), Hand);

			/* ANIMATION WHILE GRABBING */
			SERVER_UpdateAnimation(EGripEnum::CanGrab, Hand);
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
			SERVER_UpdateAnimation(EGripEnum::Open, Hand);
        }
    }
    else if (ActorFocused && ActorFocused == _LastActorFocused) {
        /* CAN BE USED */
        TArray<UActorComponent*> Components;
        ActorFocused->GetComponents(Components);
        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                SERVER_UseReleased(Component);
            }
        }
    }
	else if (_LastActorFocused) {
		TArray<UActorComponent*> Components;
		_LastActorFocused->GetComponents(Components);
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

			const UStaticMeshSocket* _GripSocket = ItemMesh->GetSocketByName(TEXT("Grip_VR_L"));

			if (_GripSocket) {
				ItemMesh->RelativeLocation = _GripSocket->RelativeLocation; 
				ItemMesh->RelativeRotation = _GripSocket->RelativeRotation;
			}

            _ActorGrabbing->SetActorEnableCollision(false);
            _ItemLeft = _ActorGrabbing;
            _ActorGrabbing = nullptr;
			SERVER_UpdateAnimation(EGripEnum::Grab, 1);
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

			const UStaticMeshSocket* _GripSocket = ItemMesh->GetSocketByName(TEXT("Grip_VR_R"));

			if (_GripSocket) {
				ItemMesh->RelativeLocation = _GripSocket->RelativeLocation;
				ItemMesh->RelativeRotation = _GripSocket->RelativeRotation;
			}

            _ActorGrabbing->SetActorEnableCollision(false);
            _ItemRight = _ActorGrabbing;
            _ActorGrabbing = nullptr;
			SERVER_UpdateAnimation(EGripEnum::Grab, 2);
        }
    }
}

/********** DROP HAND ***********/
void AVRCharacter::DropLeft() {
    if (_ItemLeft && _ItemLeft->GetComponentByClass(UGrabItem::StaticClass())) {
        SERVER_Drop(_ItemLeft, 1);
		SERVER_UpdateAnimation(EGripEnum::Open, 1);
    }
}

void AVRCharacter::DropRight() {
    if (_ItemRight && _ItemRight->GetComponentByClass(UGrabItem::StaticClass())) {
        SERVER_Drop(_ItemRight, 2);
		SERVER_UpdateAnimation(EGripEnum::Open, 2);
    }
}

bool AVRCharacter::SERVER_Drop_Validate(AActor* ItemActor, int Hand) { return true; }
void AVRCharacter::SERVER_Drop_Implementation(AActor* ItemActor, int Hand) {
    MULTI_Drop(ItemActor, Hand);
}
void AVRCharacter::MULTI_Drop_Implementation(AActor* ItemActor, int Hand) {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ItemActor->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));

    // DROP TO ALTAR
	if (ItemMesh && _ActorFocusedLeft && !bInventoryActive &&
		_ActorFocusedLeft->GetComponentByClass(UTokenHolder::StaticClass()) &&
		ItemActor->GetComponentByClass(UToken::StaticClass())) {

		UTokenHolder* Holder = Cast<UTokenHolder>(_ActorFocusedLeft->GetComponentByClass(UTokenHolder::StaticClass()));

		ItemMesh->SetMobility(EComponentMobility::Movable);
		ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		
		ItemMesh->AttachToComponent(Cast<UStaticMeshComponent>(_ActorFocusedLeft),
									FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("TablillaSocket"));

		Holder->_Tablilla = ItemActor;
	}

	else if (ItemMesh && _ActorFocusedRight && !bInventoryActive &&
		_ActorFocusedRight->GetComponentByClass(UTokenHolder::StaticClass()) &&
        ItemActor->GetComponentByClass(UToken::StaticClass())) {
	
		UTokenHolder* Holder = Cast<UTokenHolder>(_ActorFocusedRight->GetComponentByClass(UTokenHolder::StaticClass()));

		ItemMesh->SetMobility(EComponentMobility::Movable);
		ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		ItemMesh->AttachToComponent(Cast<UStaticMeshComponent>(_ActorFocusedRight),
									FAttachmentTransformRules::KeepRelativeTransform, TEXT("TablillaSocket"));

		Holder->_Tablilla = ItemActor;		
	}

    // DROP TO WORLD
    else if (ItemMesh && !bInventoryActive) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        ItemMesh->SetSimulatePhysics(true);

        ItemActor->SetActorEnableCollision(true);
    }

    // DROP TO INVENTORY
    else if (ItemMesh && bInventoryActive) SERVER_SaveItemInventory(ItemActor, Hand);

    if (Hand == 1) _ItemLeft = nullptr;
    else if (Hand == 2) _ItemRight = nullptr;
}

/**************** TRIGGER INVENTORY *************/
void AVRCharacter::ToggleInventoryInteraction(bool bActivate) {
    bInventoryActive = bActivate;

    _LeftInteractor->SetActive(bActivate);
    _LeftInteractor->SetComponentTickEnabled(bActivate);
    _LeftInteractor->SetHiddenInGame(!bActivate);
    _LeftInteractor->SetIsReplicated(bActivate);

    _RightInteractor->SetActive(bActivate);
    _RightInteractor->SetComponentTickEnabled(bActivate);
    _RightInteractor->SetHiddenInGame(!bActivate);
    _RightInteractor->SetIsReplicated(bActivate);
}

void AVRCharacter::UpdateWidgetLeftBeam() {
    if (_LeftInteractor->IsActive() && _LeftInteractor->IsOverHitTestVisibleWidget()) {
        UWidgetComponent* VRInventoryWidgetComponent = Cast<UWidgetComponent>(_LeftInteractor->GetHoveredWidgetComponent());
        if (VRInventoryWidgetComponent) {
            AVRInventory* VRInventoryActor = Cast<AVRInventory>(VRInventoryWidgetComponent->GetOwner());
            if (VRInventoryActor) {
                FVector StartPoint = _LeftHandComp->GetComponentLocation();
                FVector EndPoint = StartPoint + _LeftHandComp->GetForwardVector()*_LeftInteractor->InteractionDistance;
                FHitResult WidgetHit;

                if (GetWorld()->LineTraceSingleByChannel(WidgetHit, StartPoint, EndPoint, ECollisionChannel::ECC_Visibility)) {
                    _LeftSpline->SetStartAndEnd(StartPoint, StartPoint, WidgetHit.Location, WidgetHit.Location, false);
                }
                else {
                    _LeftSpline->SetStartAndEnd(StartPoint, StartPoint, EndPoint, EndPoint, false);
                }
            }
        }
    }
    else {
        FVector StartPoint = _LeftHandComp->GetComponentLocation();
        FVector EndPoint = StartPoint;
        _LeftSpline->SetStartAndEnd(StartPoint, StartPoint, EndPoint, EndPoint, false);
    }
}

void AVRCharacter::UpdateWidgetRightBeam() {
    if (_RightInteractor->IsActive() && _RightInteractor->IsOverHitTestVisibleWidget()) {
        UWidgetComponent* VRInventoryWidgetComponent = Cast<UWidgetComponent>(_RightInteractor->GetHoveredWidgetComponent());
        if (VRInventoryWidgetComponent) {
            AVRInventory* VRInventoryActor = Cast<AVRInventory>(VRInventoryWidgetComponent->GetOwner());
            if (VRInventoryActor) {
                FVector StartPoint = _RightHandComp->GetComponentLocation();
                FVector EndPoint = StartPoint + _RightHandComp->GetForwardVector()*_RightInteractor->InteractionDistance;
                FHitResult WidgetHit;

                if (GetWorld()->LineTraceSingleByChannel(WidgetHit, StartPoint, EndPoint, ECollisionChannel::ECC_Visibility)) {
                    _RightSpline->SetStartAndEnd(StartPoint, StartPoint, WidgetHit.Location, WidgetHit.Location, false);
                }
                else {
                    _RightSpline->SetStartAndEnd(StartPoint, StartPoint, EndPoint, EndPoint, false);
                }
            }
        }
    }
    else {
        FVector StartPoint = _RightHandComp->GetComponentLocation();
        FVector EndPoint = StartPoint;
        _RightSpline->SetStartAndEnd(StartPoint, StartPoint, EndPoint, EndPoint, false);
    }
}

/**************************************** INVENTORY **********************************************/
bool AVRCharacter::SERVER_SaveItemInventory_Validate(AActor* ItemActor, int Hand) { return true; }
void AVRCharacter::SERVER_SaveItemInventory_Implementation(AActor* ItemActor, int Hand) {
    CLIENT_ClearRadioDelegates(ItemActor);
    MULTI_SaveItemInventory(ItemActor, Hand);
}
void AVRCharacter::MULTI_SaveItemInventory_Implementation(AActor* ItemActor, int Hand) {
    if (ItemActor) {
        UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ItemActor->GetComponentByClass(
            UStaticMeshComponent::StaticClass()));
        if (ItemMesh) {
            ItemMesh->SetMobility(EComponentMobility::Movable);
            ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            _Inventory->AddItem(ItemActor);

            if (Hand == 1) { _ItemLeft = nullptr; }
            else if (Hand == 2) { _ItemRight = nullptr; }
        }
    }
}

void AVRCharacter::PickItemInventory(AActor* ItemActor, FKey KeyStruct) {
    // Logic with which while interacting with the Inventory Actor being able to pick up something from it.
    if (ItemActor) {
        // Transformado previamente un click de trigger a uno de mouse.
        if (KeyStruct == EKeys::LeftMouseButton) {
            if (_ItemLeft && _ItemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {

                /* Save hand inventory item */
                SERVER_SaveItemInventory(_ItemLeft, 1);
            }
            else if (_ItemLeft && _ItemLeft->GetComponentByClass(UHandPickItem::StaticClass())) {
                /* Drop item */
                SERVER_Drop(_ItemLeft, 1);
            }
            ////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("left click EN ITEM INVENTORY ")));
            SERVER_PickItemInventoryLeft(ItemActor);
        }
        // Transformado previamente un click de trigger a uno de mouse.
        else if (KeyStruct == EKeys::RightMouseButton) {
            if (_ItemRight && _ItemRight->GetComponentByClass(UInventoryItem::StaticClass())) {

                /* Save hand inventory item */
                SERVER_SaveItemInventory(_ItemRight, 1);
            }
            else if (_ItemRight && _ItemRight->GetComponentByClass(UHandPickItem::StaticClass())) {
                /* Drop item */
                SERVER_Drop(_ItemRight, 1);
            }
            ////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("left click EN ITEM INVENTORY ")));
            SERVER_PickItemInventoryRight(ItemActor);
        }
    }
}

bool AVRCharacter::SERVER_PickItemInventoryLeft_Validate(AActor* ItemActor) { return true; }
void AVRCharacter::SERVER_PickItemInventoryLeft_Implementation(AActor* ItemActor) {
    CLIENT_AddRadioDelegates(ItemActor);
    MULTI_PickItemInventoryLeft(ItemActor);
}
void AVRCharacter::MULTI_PickItemInventoryLeft_Implementation(AActor* ItemActor) {
    if (ItemActor) {
        UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ItemActor->GetComponentByClass(
            UStaticMeshComponent::StaticClass()));
        UInventoryItem* InventoryItemComp = Cast<UInventoryItem>(ItemActor->GetComponentByClass(
            UInventoryItem::StaticClass()));

        if (ItemMesh && InventoryItemComp) {
            ////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("ITEM TO HAND LEFT")));
            ItemMesh->SetMobility(EComponentMobility::Movable);
            ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            ItemMesh->SetSimulatePhysics(false);
            ItemMesh->AttachToComponent(_SM_LeftHand,
                                        FAttachmentTransformRules::KeepRelativeTransform,
                                        TEXT("TakeSocket"));

            const UStaticMeshSocket* _GripSocket = ItemMesh->GetSocketByName(TEXT("Grip_VR_L"));

            if (_GripSocket) {
                ItemMesh->RelativeLocation = _GripSocket->RelativeLocation;
                ItemMesh->RelativeRotation = _GripSocket->RelativeRotation;
            }

            ItemMesh->GetOwner()->SetActorHiddenInGame(false);

            _ItemLeft = ItemActor;

            /*If the item is equipped in the other hand*/
            if (_ItemRight && _ItemRight == ItemActor) {
                _ItemRight = nullptr;
                SERVER_UpdateAnimation(EGripEnum::Open, 2);
            }

            SERVER_UpdateAnimation(EGripEnum::Grab, 1);
        }
    }
}

bool AVRCharacter::SERVER_PickItemInventoryRight_Validate(AActor* ItemActor) { return true; }
void AVRCharacter::SERVER_PickItemInventoryRight_Implementation(AActor* ItemActor) {
    CLIENT_AddRadioDelegates(ItemActor);
    MULTI_PickItemInventoryRight(ItemActor);
}

void AVRCharacter::MULTI_PickItemInventoryRight_Implementation(AActor* ItemActor) {

    if (ItemActor) {
        UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ItemActor->GetComponentByClass(
            UStaticMeshComponent::StaticClass()));
        UInventoryItem* InventoryItemComp = Cast<UInventoryItem>(ItemActor->GetComponentByClass(
            UInventoryItem::StaticClass()));
        //}
        if (ItemMesh && InventoryItemComp) {
            ////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("ITEM ON HAND LEFT")));
            ItemMesh->SetMobility(EComponentMobility::Movable);
            ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            ItemMesh->SetSimulatePhysics(false);
            ItemMesh->AttachToComponent(_SM_RightHand,
                                        FAttachmentTransformRules::KeepRelativeTransform,
                                        TEXT("TakeSocket"));

            const UStaticMeshSocket* _GripSocket = ItemMesh->GetSocketByName(TEXT("Grip_VR_R"));

            if (_GripSocket) {
                ItemMesh->RelativeLocation = _GripSocket->RelativeLocation;
                ItemMesh->RelativeRotation = _GripSocket->RelativeRotation;
            }

            ItemMesh->GetOwner()->SetActorHiddenInGame(false);

            _ItemRight = ItemActor;

            /*If the item is equipped in the other hand*/
            if (_ItemLeft && _ItemLeft == ItemActor) {
                _ItemLeft = nullptr;
                SERVER_UpdateAnimation(EGripEnum::Open, 1);
            }

            SERVER_UpdateAnimation(EGripEnum::Grab, 2);
        }
    }
}

UTexture2D* AVRCharacter::GetItemTextureAt(int itemIndex) {
    return _Inventory->GetItemTextureAt(itemIndex);
}

/************ VR CHARACTER IK FEATURES *************/
void AVRCharacter::UpdateIK() {
    _HMDWorldOrientation = _PlayerCamera->GetComponentRotation();
    _LeftControllerPosition = _LeftHandComp->GetComponentLocation();
    _LeftControllerOrientation = _SM_LeftHand->GetComponentRotation();
    _RightControllerPosition = _RightHandComp->GetComponentLocation();
    _RightControllerOrientation = _SM_RightHand->GetComponentRotation();
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