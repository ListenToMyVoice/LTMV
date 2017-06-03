// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "FPCharacter.h"

#include "Inventory.h"
#include "InventoryItem.h"

#include "TutorialWidgetComponent.h"

#include "ItfUsable.h"
#include "ItfUsableItem.h"
#include "HandPickItem.h"
#include "MenuInteraction.h"
#include "InventoryWidget.h"
#include "FMODAudioComponent.h"

AFPCharacter::AFPCharacter(const FObjectInitializer& OI) : Super(OI) {
    _Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
    /*RAYCAST PARAMETERS*/
    _RayParameter = 250.0f;

    _PlayerCamera->bUsePawnControlRotation = true;
    _PlayerCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("FPVCamera"));
    _Inventory->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("inventory"));
    _StepsAudioComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("Foot"));
	_BreathAudioComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("spine_03"));
	_FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
	_FirstPersonMesh->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform);

    GetCharacterMovement()->MaxWalkSpeed = 200.0f;

    //TUTORIAL
    _Tutorial0 = OI.CreateDefaultSubobject<UTutorialWidgetComponent>(this, TEXT("TutorialWidget0"));
    _Tutorial1 = OI.CreateDefaultSubobject<UTutorialWidgetComponent>(this, TEXT("TutorialWidget1"));
    _Tutorial2 = OI.CreateDefaultSubobject<UTutorialWidgetComponent>(this, TEXT("TutorialWidget2"));
    _Tutorial3 = OI.CreateDefaultSubobject<UTutorialWidgetComponent>(this, TEXT("TutorialWidget3"));
    _Tutorial4 = OI.CreateDefaultSubobject<UTutorialWidgetComponent>(this, TEXT("TutorialWidget4"));
    _Tutorial5 = OI.CreateDefaultSubobject<UTutorialWidgetComponent>(this, TEXT("TutorialWidget5"));
    _Tutorial6 = OI.CreateDefaultSubobject<UTutorialWidgetComponent>(this, TEXT("TutorialWidget6"));
    _Tutorial7 = OI.CreateDefaultSubobject<UTutorialWidgetComponent>(this, TEXT("TutorialWidget7"));
    _Tutorial8 = OI.CreateDefaultSubobject<UTutorialWidgetComponent>(this, TEXT("TutorialWidget8"));

	//Tutorial attachments
	_Tutorial0->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform, FName("tutorialwidget0"));
	_Tutorial1->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform, FName("tutorialwidget1"));
	_Tutorial2->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform, FName("tutorialwidget2"));
	_Tutorial3->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform, FName("tutorialwidget3"));
	_Tutorial4->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform, FName("tutorialwidget4"));
	_Tutorial5->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform, FName("tutorialwidget5"));
	_Tutorial6->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform, FName("tutorialwidget6"));
	_Tutorial7->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform, FName("tutorialwidget7"));
	_Tutorial8->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform, FName("tutorialwidget8"));
}

void AFPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInput) {
    Super::SetupPlayerInputComponent(PlayerInput);

    /* MOVEMENT */
    PlayerInput->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInput->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
    PlayerInput->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInput->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

    /* ACTIONS */
    PlayerInput->BindAction("TakeDropRight", IE_Released, this, &AFPCharacter::TakeDropRight);
    PlayerInput->BindAction("TakeDropLeft", IE_Released, this, &AFPCharacter::TakeDropLeft);

    PlayerInput->BindAction("Use", IE_Pressed, this, &AFPCharacter::UsePressed);
    PlayerInput->BindAction("Use", IE_Released, this, &AFPCharacter::UseReleased);

    PlayerInput->BindAction("ToggleInventory", IE_Pressed, this, &AFPCharacter::ToggleInventory);

}

void AFPCharacter::BeginPlay() {
    Super::BeginPlay();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController && PlayerController->IsLocalPlayerController()) {
        /* HUD */
        UUserWidget* HUD = CreateWidget<UUserWidget>(PlayerController, _HUDClass);
        if (HUD) HUD->AddToViewport();
		
    }

	//TUTORIAL starters
	_Tutorial0->SetVisibility(false);
	_Tutorial1->SetVisibility(false);
	_Tutorial2->SetVisibility(false);
	_Tutorial3->SetVisibility(false);
	_Tutorial4->SetVisibility(false);
	_Tutorial5->SetVisibility(false);
	_Tutorial6->SetVisibility(false);
	_Tutorial7->SetVisibility(false);
	_Tutorial8->SetVisibility(false);
	_TutorialLevelEnum = ETutorialLevelEnum::TL_Disabled;
	_IsTutorialRunning = false;

}

void AFPCharacter::AfterPossessed(bool SetInventory) {
    Super::AfterPossessed(SetInventory);

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController->IsLocalPlayerController()) {
        if (SetInventory) {
            _InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController, _InventoryUIClass);
            if (_InventoryWidget) {
                _InventoryWidget->AddToViewport(); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
                _InventoryWidget->SetVisibility(ESlateVisibility::Hidden); // Set it to hidden so its not open on spawn.
                _IsInventoryHidden = true;
				_TutorialTimer = 0.0f;
            }

			if (_TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled) {
				//Tutorial at bunker
				_Tutorial0->SetVisibility(true);
				_TutorialLevelEnum = ETutorialLevelEnum::TL_0;
				_IsTutorialRunning = true;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("EN EL BUNKER: ENUM A TL_0")));
			}
		}
		else {
			/*
			if (_TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled) {
				//Tutorial at cave
				_Tutorial7->SetVisibility(true);
				_TutorialLevelEnum = ETutorialLevelEnum::TL_0_1;
				_IsTutorialRunning = true;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Empieza EL TUTORIAL: ENUM A TL_0_1")));
			}
			*/
		}
    }
}
template<typename TEnum>
static FORCEINLINE FString GetEnumValueToString(const FString& Name, TEnum Value){
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr){
		return FString("Invalid");
	}
	return enumPtr->GetNameByValue((int64)Value).ToString();
}
void AFPCharacter::Tick(float DeltaSeconds) {

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("valor del enum: %d y del tutorial %s"), _IsTutorialRunning, *GetEnumValueToString<ETutorialLevelEnum>("ETutorialLevelEnum", _TutorialLevelEnum)));
    Super::Tick(DeltaSeconds);
    //_StepsAudioComp->SetParameter(FName("humedad"), 0.9);
    Raycasting();

	if (_TutorialLevelEnum == ETutorialLevelEnum::TL_0_1) {
		if (_TutorialTimer <= 320.0f) {
			_TutorialTimer += 1.0f;
		}
		else {
			_Tutorial7->SetVisibility(false);
			_IsTutorialRunning = false;
			_TutorialTimer = 0.0f;
		}
	}
	if (_TutorialLevelEnum == ETutorialLevelEnum::TL_8) {
		if (_TutorialTimer <= 320.0f) {
			_TutorialTimer += 1.0f;
		}
		else {
			_Tutorial8->SetVisibility(false);
			_IsTutorialRunning = false;
			_TutorialLevelEnum = ETutorialLevelEnum::TL_Disabled;
		}
	}
}

FHitResult AFPCharacter::Raycasting() {
    bool bHitRayCastFlag = false;
    FCollisionQueryParams CollisionInfo;

    FVector StartRaycast = _PlayerCamera->GetComponentLocation();
    FVector EndRaycast = _PlayerCamera->GetForwardVector() * _RayParameter + StartRaycast;

    bHitRayCastFlag = GetWorld()->LineTraceSingleByChannel(_HitResult, StartRaycast, EndRaycast, ECC_Visibility, CollisionInfo);

    //DrawDebugLine(GetWorld(), StartRaycast, EndRaycast, FColor(255, 0, 0), false, -1.0f, (uint8)'\000', 0.8f);

    if (bHitRayCastFlag && _HitResult.Actor.IsValid()) {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *_HitResult.Actor->GetName()));
        UActorComponent* actorComponent = _HitResult.GetComponent();

        TArray<UActorComponent*> components = actorComponent->GetOwner()->GetComponentsByClass(UActorComponent::StaticClass());

        for (UActorComponent* component : components) {

            //Highlight outline colors:
            //GREEN: 252 | BLUE: 253 | ORANGE: 254 | WHITE: 255
            if (component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                _LastMeshFocused = Cast<UStaticMeshComponent>(component->GetOwner()->GetComponentByClass(
                    UStaticMeshComponent::StaticClass()));

                _LastMeshFocused->SetRenderCustomDepth(true);
                _LastMeshFocused->SetCustomDepthStencilValue(252);
                bInventoryItemHit = true;
            }
            else if (component->GetClass() == UInventoryItem::StaticClass()) {
                _LastMeshFocused = Cast<UStaticMeshComponent>(component->GetOwner()->GetComponentByClass(
                    UStaticMeshComponent::StaticClass()));

                _LastMeshFocused->SetRenderCustomDepth(true);
                _LastMeshFocused->SetCustomDepthStencilValue(253);
                bInventoryItemHit = true;
				
				//Pasa a Tutorial TL_01 
				if (_TutorialLevelEnum == ETutorialLevelEnum::TL_0 && _TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled) {
					_Tutorial0->SetVisibility(false);
					_Tutorial1->SetVisibility(true);
					_IsTutorialRunning = true;
					_TutorialLevelEnum = ETutorialLevelEnum::TL_1;
				}
				
            }
            else if (component->GetClass() == UHandPickItem::StaticClass()) {
                _LastMeshFocused = Cast<UStaticMeshComponent>(component->GetOwner()->GetComponentByClass(
                    UStaticMeshComponent::StaticClass()));

                _LastMeshFocused->SetRenderCustomDepth(true);
                _LastMeshFocused->SetCustomDepthStencilValue(255);
                bInventoryItemHit = true;
            }

            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *_HitResult.Actor->GetName()));
        }
    }

    //If Raycast is not hitting any actor, disable the outline
    if (bInventoryItemHit && _HitResult.Actor != _LastMeshFocused->GetOwner()) {

        _LastMeshFocused->SetCustomDepthStencilValue(0);
        _LastMeshFocused->SetRenderCustomDepth(false);

        bInventoryItemHit = false;
    }


    return _HitResult;
}

/****************************************** ACTION MAPPINGS **************************************/
/************** USE *************/
void AFPCharacter::UsePressed() {
    /* RAYCASTING DETECTION */
    if (_HitResult.GetActor()) {
        TArray<UActorComponent*> Components;
        _HitResult.GetActor()->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                SERVER_UsePressed(Component);
            }
        }
    }
}

void AFPCharacter::UseReleased() {
    /* RAYCASTING DETECTION */
    if (_HitResult.GetActor()) {
        TArray<UActorComponent*> Components;
        _HitResult.GetActor()->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                SERVER_UseReleased(Component);
            }
        }
    }
}

/******** USE ITEM LEFT *********/
void AFPCharacter::UseLeftPressed(bool IsMenuHidden) {
    if (IsMenuHidden) {
        if (_ItemLeft && _IsInventoryHidden) {

			//Tutorial pasa a TL_06
			if (_TutorialLevelEnum == ETutorialLevelEnum::TL_5 && _TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled) {
				_Tutorial5->SetVisibility(false);
				_Tutorial6->SetVisibility(true);
				_IsTutorialRunning = true;
				_TutorialLevelEnum = ETutorialLevelEnum::TL_6;
			}
            TArray<UActorComponent*> Components;
            _ItemLeft->GetComponents(Components);

            for (UActorComponent* Component : Components) {
                if (Component->GetClass()->ImplementsInterface(UItfUsableItem::StaticClass())) {
                    IItfUsableItem* ItfObject = Cast<IItfUsableItem>(Component);
                    if (ItfObject) ItfObject->Execute_UseItemPressed(Component);
                }
            }
        }
    }
    else _MenuInteractionComp->PressPointer();
}

void AFPCharacter::UseLeftReleased(bool IsMenuHidden) {
    if (IsMenuHidden) {
        if (_ItemLeft && _IsInventoryHidden) {

            TArray<UActorComponent*> Components;
            _ItemLeft->GetComponents(Components);

            for (UActorComponent* Component : Components) {
                if (Component->GetClass()->ImplementsInterface(UItfUsableItem::StaticClass())) {
                    IItfUsableItem* ItfObject = Cast<IItfUsableItem>(Component);
                    if (ItfObject) ItfObject->Execute_UseItemReleased(Component);
                }
            }
        }
    }
    else _MenuInteractionComp->ReleasePointer();
}

/******* USE ITEM RIGHT *********/
void AFPCharacter::UseRightPressed(bool IsMenuHidden) {
    if (_ItemRight && _IsInventoryHidden) {
		
		//Tutorial pasa a TL_06
		if (_TutorialLevelEnum == ETutorialLevelEnum::TL_5 && _TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled) {
			_Tutorial5->SetVisibility(false);
			_Tutorial6->SetVisibility(true);
			_IsTutorialRunning = true;
			_TutorialLevelEnum = ETutorialLevelEnum::TL_6;
		}
		
        TArray<UActorComponent*> Components;
        _ItemRight->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsableItem::StaticClass())) {
                IItfUsableItem* ItfObject = Cast<IItfUsableItem>(Component);
                if (ItfObject) ItfObject->Execute_UseItemPressed(Component);
            }
        }
    }
}

void AFPCharacter::UseRightReleased(bool IsMenuHidden) {
    if (_ItemRight && _IsInventoryHidden) {
		
        TArray<UActorComponent*> Components;
        _ItemRight->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsableItem::StaticClass())) {
                IItfUsableItem* ItfObject = Cast<IItfUsableItem>(Component);
                if (ItfObject) ItfObject->Execute_UseItemReleased(Component);
            }
        }
    }
}

/********** TAKE & DROP RIGHT HAND ***********/
void AFPCharacter::TakeDropRight() {
    AActor* ActorFocused = GetItemFocused();
    if (ActorFocused) {
        if (ActorFocused->GetComponentByClass(UInventoryItem::StaticClass())) {
			
			//Tutorial pasar a TL_02
			if (_TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled && _TutorialLevelEnum == ETutorialLevelEnum::TL_1) {
				_Tutorial1->SetVisibility(false);
				_Tutorial2->SetVisibility(true);
				_IsTutorialRunning = true;
				_TutorialLevelEnum = ETutorialLevelEnum::TL_2;
			}
			//Tutorial pasar a TL_08
			if (_TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled && _TutorialLevelEnum == ETutorialLevelEnum::TL_6) {
				_Tutorial6->SetVisibility(false);
				_Tutorial8->SetVisibility(true);
				_IsTutorialRunning = true;
				_TutorialLevelEnum = ETutorialLevelEnum::TL_8;
				_TutorialTimer = 0.0f;
			}
			
            /* Save scenary inventory item */
            SERVER_SaveItemInventory(ActorFocused, 0);
        }
        else if (ActorFocused->GetComponentByClass(UHandPickItem::StaticClass())) {
            if (_ItemRight && _ItemRight->GetComponentByClass(UHandPickItem::StaticClass())) {
                /* Replace item */
                SERVER_Drop(_ItemRight, 2);
                SERVER_TakeRight(ActorFocused);
				UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ActorFocused->GetComponentByClass(
					UStaticMeshComponent::StaticClass()));
				const UStaticMeshSocket* _GripSocket = ItemMesh->GetSocketByName(TEXT("Grip_R"));
				ItemMesh->AttachToComponent(_FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform,
					"GripPoint_R");
				if (_GripSocket)
				{
					ItemMesh->RelativeLocation = _GripSocket->RelativeLocation;
					ItemMesh->RelativeRotation = _GripSocket->RelativeRotation;
				}
				GrabbingRight = true;
            }
            else if (_ItemRight && _ItemRight->GetComponentByClass(UInventoryItem::StaticClass())) {
                /* Save hand inventory item */
                SERVER_SaveItemInventory(_ItemRight, 2);
            }
            else if (!_ItemRight) {
                /* Take item */
                SERVER_TakeRight(ActorFocused);
				UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ActorFocused->GetComponentByClass(
					UStaticMeshComponent::StaticClass()));
				const UStaticMeshSocket* _GripSocket = ItemMesh->GetSocketByName(TEXT("Grip_R"));
				ItemMesh->AttachToComponent(_FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform,
					"GripPoint_R");
				if (_GripSocket)
				{
					ItemMesh->RelativeLocation = _GripSocket->RelativeLocation;
					ItemMesh->RelativeRotation = _GripSocket->RelativeRotation;
				}
				GrabbingRight = true;
            }
        }
    }
    else if (_ItemRight && _ItemRight->GetComponentByClass(UHandPickItem::StaticClass())) {
        /* Drop item */
        SERVER_Drop(_ItemRight, 2);
		GrabbingRight = false;
    }
    else if (_ItemRight && _ItemRight->GetComponentByClass(UInventoryItem::StaticClass())) {
		
		//Tutorial pasar a TL_02
		if (_TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled && _TutorialLevelEnum == ETutorialLevelEnum::TL_1) {
			_Tutorial1->SetVisibility(false);
			_Tutorial2->SetVisibility(true);
			_IsTutorialRunning = true;
			_TutorialLevelEnum = ETutorialLevelEnum::TL_2;
		}
		
		//Tutorial pasar a TL_08
		if (_TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled && _TutorialLevelEnum == ETutorialLevelEnum::TL_6) {
			_Tutorial6->SetVisibility(false);
			_Tutorial8->SetVisibility(true);
			_IsTutorialRunning = true;
			_TutorialLevelEnum = ETutorialLevelEnum::TL_8;
			_TutorialTimer = 0.0f;
		}
        /* Save hand inventory item */
        SERVER_SaveItemInventory(_ItemRight, 2);
    }
}

/********** TAKE & DROP LEFT HAND ***********/
void AFPCharacter::TakeDropLeft() {
    AActor* ActorFocused = GetItemFocused();
    if (ActorFocused) {
        if (ActorFocused->GetComponentByClass(UInventoryItem::StaticClass())) {
			
			//Tutorial pasar a TL_02
			if (_TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled && _TutorialLevelEnum == ETutorialLevelEnum::TL_1) {
				_Tutorial1->SetVisibility(false);
				_Tutorial2->SetVisibility(true);
				_IsTutorialRunning = true;
				_TutorialLevelEnum = ETutorialLevelEnum::TL_2;
			}
			//Tutorial pasar a TL_08
			if (_TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled && _TutorialLevelEnum == ETutorialLevelEnum::TL_6) {
				_Tutorial6->SetVisibility(false);
				_Tutorial8->SetVisibility(true);
				_IsTutorialRunning = true;
				_TutorialLevelEnum = ETutorialLevelEnum::TL_8;
				_TutorialTimer = 0.0f;
			}
            /* Save scenary inventory item */
            SERVER_SaveItemInventory(ActorFocused, 0);
        }
        else if (ActorFocused->GetComponentByClass(UHandPickItem::StaticClass())) {
            if (_ItemLeft && _ItemLeft->GetComponentByClass(UHandPickItem::StaticClass())) {
                /* Replace item */
                SERVER_Drop(_ItemLeft, 1);
                SERVER_TakeLeft(ActorFocused);
				UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ActorFocused->GetComponentByClass(
					UStaticMeshComponent::StaticClass()));
				const UStaticMeshSocket* _GripSocket = ItemMesh->GetSocketByName(TEXT("Grip_L"));
				ItemMesh->AttachToComponent(_FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform,
					"GripPoint_L");
				if (_GripSocket)
				{
					ItemMesh->RelativeLocation = _GripSocket->RelativeLocation;
					ItemMesh->RelativeRotation = _GripSocket->RelativeRotation;
				}
				GrabbingLeft = true;
            }
            else if (_ItemLeft && _ItemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {
                /* Save hand inventory item */
                SERVER_SaveItemInventory(_ItemLeft, 1);
            }
            else if (!_ItemLeft) {
                /* Take item */
                SERVER_TakeLeft(ActorFocused);
				UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ActorFocused->GetComponentByClass(
					UStaticMeshComponent::StaticClass()));
				const UStaticMeshSocket* _GripSocket = ItemMesh->GetSocketByName(TEXT("Grip_L"));
				ItemMesh->AttachToComponent(_FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform,
					"GripPoint_L");
				if (_GripSocket)
				{
					ItemMesh->RelativeLocation = _GripSocket->RelativeLocation;
					ItemMesh->RelativeRotation = _GripSocket->RelativeRotation;
				}
				GrabbingLeft = true;
            }
        }
    }
    else if (_ItemLeft && _ItemLeft->GetComponentByClass(UHandPickItem::StaticClass())) {
        /* Drop item */
        SERVER_Drop(_ItemLeft, 1);
		GrabbingLeft = false;
    }
    else if (_ItemLeft && _ItemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {
		
		//Tutorial pasar a TL_02
		if (_TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled && _TutorialLevelEnum == ETutorialLevelEnum::TL_1) {
			_Tutorial1->SetVisibility(false);
			_Tutorial2->SetVisibility(true);
			_IsTutorialRunning = true;
			_TutorialLevelEnum = ETutorialLevelEnum::TL_2;
		}
		//Tutorial pasar a TL_08
		if (_TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled && _TutorialLevelEnum == ETutorialLevelEnum::TL_6) {
			_Tutorial6->SetVisibility(false);
			_Tutorial8->SetVisibility(true);
			_IsTutorialRunning = true;
			_TutorialLevelEnum = ETutorialLevelEnum::TL_8;
			_TutorialTimer = 0.0f;
		}
        /* Save hand inventory item */
        SERVER_SaveItemInventory(_ItemLeft, 1);
    }
}

/**************** TRIGGER INVENTORY *************/
/*** SHOW INVENTORY ***/
void AFPCharacter::ToggleInventory() {
    if (!_MenuInteractionComp->IsActive()) {
        APlayerController* PlayerController = Cast<APlayerController>(GetController());
        if (PlayerController && _InventoryWidget) {
            if (_IsInventoryHidden) {
                _InventoryWidget->SetVisibility(ESlateVisibility::Visible);
                PlayerController->bShowMouseCursor = true;
                PlayerController->bEnableClickEvents = true;
                PlayerController->bEnableMouseOverEvents = true;

                FInputModeGameAndUI Mode = FInputModeGameAndUI();
                Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
                Mode.SetWidgetToFocus(_InventoryWidget->TakeWidget());
                PlayerController->SetInputMode(Mode);
				
				//Tutorial pasa a TL_03
				if (_TutorialLevelEnum == ETutorialLevelEnum::TL_2 && _TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled) {
					_Tutorial2->SetVisibility(false);
					_Tutorial3->SetVisibility(true);
					_IsTutorialRunning = true;
					_TutorialLevelEnum = ETutorialLevelEnum::TL_3;
				}
				
            }
            else {
                _InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
                PlayerController->bShowMouseCursor = false;
                PlayerController->bEnableClickEvents = false;
                PlayerController->bEnableMouseOverEvents = false;
                PlayerController->SetInputMode(FInputModeGameOnly());
				
				//Tutorial pasa a TL_05
				if (_TutorialLevelEnum == ETutorialLevelEnum::TL_4 && _TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled) {
					_Tutorial4->SetVisibility(false);
					_Tutorial5->SetVisibility(true);
					_IsTutorialRunning = true;
					_TutorialLevelEnum = ETutorialLevelEnum::TL_5;
				}
				
            }
            _IsInventoryHidden = !_IsInventoryHidden;
        }
    }
}

/**************************************** INVENTORY **********************************************/
bool AFPCharacter::SERVER_SaveItemInventory_Validate(AActor* ItemActor, int Hand) { return true; }
void AFPCharacter::SERVER_SaveItemInventory_Implementation(AActor* ItemActor, int Hand) {
    CLIENT_ClearRadioDelegates(ItemActor);
    MULTI_SaveItemInventory(ItemActor, Hand);
}
void AFPCharacter::MULTI_SaveItemInventory_Implementation(AActor* ItemActor, int Hand) {
    if (ItemActor) {
        UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ItemActor->GetComponentByClass(
            UStaticMeshComponent::StaticClass()));
		if (ItemMesh) {
			ItemMesh->SetMobility(EComponentMobility::Movable);
			_Inventory->AddItem(ItemActor);

			if (Hand == 1) { _ItemLeft = nullptr; GrabbingLeft = false; }
			else if (Hand == 2) { _ItemRight = nullptr; GrabbingRight = false; }
        }
    }
}

void AFPCharacter::PickItemInventory(AActor* ItemActor, FKey KeyStruct) {
    if (ItemActor) {
        if (KeyStruct == EKeys::LeftMouseButton) {
            if (_ItemLeft && _ItemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {
				
				//Tutorial pasa a TL_04
				if (_TutorialLevelEnum == ETutorialLevelEnum::TL_3 && _TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled) {
					_Tutorial3->SetVisibility(false);
					_Tutorial4->SetVisibility(true);
					_IsTutorialRunning = true;
					_TutorialLevelEnum = ETutorialLevelEnum::TL_4;
				}
				
                /* Save hand inventory item */
                SERVER_SaveItemInventory(_ItemLeft, 1);
            }
            else if (_ItemLeft && _ItemLeft->GetComponentByClass(UHandPickItem::StaticClass())) {
                /* Drop item */
				SERVER_Drop(_ItemLeft, 1);
            }
            SERVER_PickItemInventoryLeft(ItemActor);
        }
        else if (KeyStruct == EKeys::RightMouseButton) {
            if (_ItemRight && _ItemRight->GetComponentByClass(UInventoryItem::StaticClass())) {
				
				//Tutorial pasa a TL_04
				if (_TutorialLevelEnum == ETutorialLevelEnum::TL_3 && _TutorialLevelEnum != ETutorialLevelEnum::TL_Disabled) {
					_Tutorial3->SetVisibility(false);
					_Tutorial4->SetVisibility(true);
					_IsTutorialRunning = true;
					_TutorialLevelEnum = ETutorialLevelEnum::TL_4;
				}
				
                /* Save hand inventory item */
                SERVER_SaveItemInventory(_ItemRight, 2);
            }
            else if (_ItemRight && _ItemRight->GetComponentByClass(UHandPickItem::StaticClass())) {
                /* Drop item */
                SERVER_Drop(_ItemRight, 2);
            }
            SERVER_PickItemInventoryRight(ItemActor);
        }
    }
}

bool AFPCharacter::SERVER_PickItemInventoryLeft_Validate(AActor* ItemActor) { return true; }
void AFPCharacter::SERVER_PickItemInventoryLeft_Implementation(AActor* ItemActor) {
    CLIENT_AddRadioDelegates(ItemActor);
    MULTI_PickItemInventoryLeft(ItemActor);
}
void AFPCharacter::MULTI_PickItemInventoryLeft_Implementation(AActor* ItemActor) {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ItemActor->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));
    UInventoryItem* InventoryItemComp = Cast<UInventoryItem>(ItemActor->GetComponentByClass(
        UInventoryItem::StaticClass()));

    if (ItemMesh && InventoryItemComp) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->SetSimulatePhysics(false);
		/*
        ItemMesh->AttachToComponent(GetMesh(),
                                    FAttachmentTransformRules::KeepRelativeTransform,
                                    TEXT("itemHand_l"));

        ItemMesh->RelativeLocation = InventoryItemComp->_locationAttachFromInventory_L;
        ItemMesh->RelativeRotation = InventoryItemComp->_rotationAttachFromInventory_L;
		*/
		const UStaticMeshSocket* _GripSocket = ItemMesh->GetSocketByName(TEXT("Grip_L"));
		ItemMesh->AttachToComponent(_FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform,
			"GripPoint_L");
		if (_GripSocket)
		{
			ItemMesh->RelativeLocation = _GripSocket->RelativeLocation;
			ItemMesh->RelativeRotation = _GripSocket->RelativeRotation;
		}

        ItemMesh->GetOwner()->SetActorHiddenInGame(false);

        _ItemLeft = ItemActor;
		GrabbingLeft = true;

        /*If the item is equipped in the other hand*/
		if (_ItemRight && _ItemRight == ItemActor) {
			_ItemRight = nullptr;
			GrabbingRight = false;
		}
    }
}

bool AFPCharacter::SERVER_PickItemInventoryRight_Validate(AActor* ItemActor) { return true; }
void AFPCharacter::SERVER_PickItemInventoryRight_Implementation(AActor* ItemActor) {
    CLIENT_AddRadioDelegates(ItemActor);
    MULTI_PickItemInventoryRight(ItemActor);
}

void AFPCharacter::MULTI_PickItemInventoryRight_Implementation(AActor* ItemActor) {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ItemActor->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));
    UInventoryItem* InventoryItemComp = Cast<UInventoryItem>(ItemActor->GetComponentByClass(
        UInventoryItem::StaticClass()));

    if (ItemMesh && InventoryItemComp) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->SetSimulatePhysics(false);
		/*
        ItemMesh->AttachToComponent(GetMesh(),
                                    FAttachmentTransformRules::KeepRelativeTransform,
                                    TEXT("itemHand_r"));

        ItemMesh->RelativeLocation = InventoryItemComp->_locationAttachFromInventory_R;
        ItemMesh->RelativeRotation = InventoryItemComp->_rotationAttachFromInventory_R;
		*/
		const UStaticMeshSocket* _GripSocket = ItemMesh->GetSocketByName(TEXT("Grip_R"));
		ItemMesh->AttachToComponent(_FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform,
			"GripPoint_R");
		if (_GripSocket)
		{
			ItemMesh->RelativeLocation = _GripSocket->RelativeLocation;
			ItemMesh->RelativeRotation = _GripSocket->RelativeRotation;
		}

		ItemMesh->GetOwner()->SetActorHiddenInGame(false);

        _ItemRight = ItemActor;		
		GrabbingRight = true;

        /*If the item is equipped in the other hand*/
		if (_ItemLeft && _ItemLeft == ItemActor) {
			_ItemLeft = nullptr;
			GrabbingLeft = false;
		}
    }
}

/****************************************** AUXILIAR FUNCTIONS ***********************************/
void AFPCharacter::ToggleTutorial() {
	_Tutorial0->SetVisibility(false);
	_Tutorial1->SetVisibility(false);
	_Tutorial2->SetVisibility(false);
	_Tutorial3->SetVisibility(false);
	_Tutorial4->SetVisibility(false);
	_Tutorial5->SetVisibility(false);
	_Tutorial6->SetVisibility(false);
	_Tutorial7->SetVisibility(false);
	_Tutorial8->SetVisibility(false);
	_IsTutorialRunning = false;
}
void AFPCharacter::ToggleMenuInteraction(bool Activate) {
    if (!_IsInventoryHidden) ToggleInventory();
	//if (_IsTutorialRunning) ToggleTutorial();

    Super::ToggleMenuInteraction(Activate);
}

UTexture2D* AFPCharacter::GetItemTextureAt(int itemIndex) {
    return _Inventory->GetItemTextureAt(itemIndex);
}

AActor* AFPCharacter::GetItemFocused() {
    AActor* ActorFocused = _HitResult.GetActor();
    if (ActorFocused && ActorFocused->GetComponentByClass(UStaticMeshComponent::StaticClass()) &&
        (ActorFocused->GetComponentByClass(UInventoryItem::StaticClass()) ||
         ActorFocused->GetComponentByClass(UHandPickItem::StaticClass()))) {
        return ActorFocused;
    }
    return nullptr;
}

bool AFPCharacter::GetGrabbingLeft() {
	return GrabbingLeft;
}

bool AFPCharacter::GetGrabbingRight() {
	return GrabbingRight;
}