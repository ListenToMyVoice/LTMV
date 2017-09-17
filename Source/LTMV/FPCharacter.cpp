// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "FPCharacter.h"

#include "Inventory.h"
#include "InventoryItem.h"

#include "Tutorial.h"
#include "TutorialVR.h"

#include "ItfUsable.h"
#include "ItfUsableItem.h"
#include "HandPickItem.h"
#include "TokenHolder.h"
#include "Token.h"
#include "MenuInteraction.h"
#include "InventoryWidget.h"
#include "FMODAudioComponent.h"
#include "NWGameInstance.h"

AFPCharacter::AFPCharacter(const FObjectInitializer& OI) : Super(OI) {
	
    _Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
	_Tutorial = CreateDefaultSubobject<UTutorial>(TEXT("Tutorial"));
	//_TutorialVR = CreateDefaultSubobject<UTutorialVR>(TEXT("TutorialVR"));

    /*RAYCAST PARAMETERS*/
    _RayParameter = 250.0f;

    _PlayerCamera->bUsePawnControlRotation = true;
    _PlayerCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("FPVCamera"));
    _Inventory->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("inventory"));
    _StepsAudioComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("Foot"));
	//_BreathAudioComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("spine_03"));
	_FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
	_FirstPersonMesh->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform);
	//_TutorialVR->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform);

	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
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
	PlayerInput->BindAction("FadeDisplay", IE_Pressed, this, &AFPCharacter::FadeDisplay);
}

void AFPCharacter::FadeDisplay() {// T
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

void AFPCharacter::BeginPlay() {
    Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController && PlayerController->IsLocalPlayerController()) {
        // HUD
        HUD = CreateWidget<UUserWidget>(PlayerController, _HUDClass);
        if (HUD) HUD->AddToViewport();

		HUD2 = CreateWidget<UUserWidget>(PlayerController, _HUDClass2);
		if (HUD2) HUD2->AddToViewport();
		HUD2->SetVisibility(ESlateVisibility::Hidden);

        // Client inventory widget overlap (this is a guess).
        _InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController, _InventoryUIClass);
        if (_InventoryWidget) _InventoryWidget->AddToViewport();
        _InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
        _IsInventoryHidden = true;
    }
}

void AFPCharacter::AfterPossessed(bool SetInventory, bool respawning) {

    Super::AfterPossessed(SetInventory, respawning);

	UNWGameInstance* gameInstance = Cast<UNWGameInstance>(GetGameInstance());
	_Tutorial->SetLanguage(gameInstance->_PlayerInfoSaved.Language);//Set language
	//_TutorialVR->SetLanguage(gameInstance->_PlayerInfoSaved.Language);
	if (respawning) {
		_isTutorialEnabled = false;
		_Tutorial->Hide();
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController->IsLocalPlayerController()) {
		if (SetInventory) {
            ////GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("possesed with inventory"));
			if (_isTutorialEnabled) {
				_Tutorial->Next(PlayerController, 0, false);//Updating to next tutorial widget

				FVector Location = _PlayerCamera->GetComponentLocation() +
					(_PlayerCamera->GetForwardVector().GetSafeNormal() * 200);
				//_TutorialVR->Next(Location, _PlayerCamera->GetComponentRotation(), 0);//Tutorial at bunker/lab
			}

            if (!_InventoryWidget) {
                _InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController, _InventoryUIClass);
                if (_InventoryWidget) {
                    _InventoryWidget->AddToViewport(); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
                    _InventoryWidget->SetVisibility(ESlateVisibility::Hidden); // Set it to hidden so its not open on spawn.
                    _IsInventoryHidden = true;
                }
            }
		}
	}
	if (!SetInventory) {
		if (_isTutorialEnabled) {
			////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("TUTORIAL EN LA CUEVA: ")));
			_Tutorial->StartTutorial(PlayerController);//Starting tutorial at lobby

			if (PlayerController->IsLocalPlayerController()) {
				//_TutorialVR->StartTutorial(_PlayerCamera);//Start tutorial at lobby
			}
		}
	}
}

void AFPCharacter::Tick(float DeltaSeconds) {
	
	Super::Tick(DeltaSeconds);
    //_StepsAudioComp->SetParameter(FName("humedad"), 0.9);
    Raycasting();
	/*
	FVector StartRaycast = _FirstPersonMesh->GetSocketByName("GripPoint_L")->GetSocketLocation(_FirstPersonMesh);
	FVector EndRaycast = _PlayerCamera->GetForwardVector() * 200.f + StartRaycast;

	DrawDebugLine(GetWorld(), StartRaycast, EndRaycast, FColor(0, 255, 0), false, -1.f, (uint8)'\000', 0.8f);
	*/
}
	
FHitResult AFPCharacter::Raycasting() {
    bool bHitRayCastFlag = false;
    FCollisionQueryParams CollisionInfo;

    FVector StartRaycast = _PlayerCamera->GetComponentLocation();
    FVector EndRaycast = _PlayerCamera->GetForwardVector() * _RayParameter + StartRaycast;

    bHitRayCastFlag = GetWorld()->LineTraceSingleByChannel(_HitResult, StartRaycast, EndRaycast, ECC_Visibility, CollisionInfo);

    //DrawDebugLine(GetWorld(), StartRaycast, EndRaycast, FColor(255, 0, 0), false, -1.0f, (uint8)'\000', 0.8f);

    if (bHitRayCastFlag && _HitResult.Actor.IsValid()) {
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

				if (HUD && HUD2) {
					HUD->SetVisibility(ESlateVisibility::Hidden);
					HUD2->SetVisibility(ESlateVisibility::Visible);
				}
            }
            else if (component->GetClass() == UInventoryItem::StaticClass()) {
                _LastMeshFocused = Cast<UStaticMeshComponent>(component->GetOwner()->GetComponentByClass(
                    UStaticMeshComponent::StaticClass()));

                _LastMeshFocused->SetRenderCustomDepth(true);
                _LastMeshFocused->SetCustomDepthStencilValue(253);
                bInventoryItemHit = true;

				if (_isTutorialEnabled) {
					APlayerController* PlayerController = Cast<APlayerController>(GetController());
					_Tutorial->Next(PlayerController, 1, false);//NExt widget of tutorial

					FVector Location;
					Location = _HitResult.Actor->GetActorLocation()
						+ (_HitResult.Actor->GetActorForwardVector().GetSafeNormal() * 90)
						+ (_HitResult.Actor->GetActorUpVector().GetSafeNormal() * -30);
					//_TutorialVR->Next(Location, _PlayerCamera->GetComponentRotation(), 1);//Tutorial at bunker/lab
				}
				if (HUD && HUD2) {
					HUD->SetVisibility(ESlateVisibility::Hidden);
					HUD2->SetVisibility(ESlateVisibility::Visible);
				}
				
            }
            else if (component->GetClass() == UHandPickItem::StaticClass()) {
                _LastMeshFocused = Cast<UStaticMeshComponent>(component->GetOwner()->GetComponentByClass(
                    UStaticMeshComponent::StaticClass()));

                _LastMeshFocused->SetRenderCustomDepth(true);
                _LastMeshFocused->SetCustomDepthStencilValue(255);
                bInventoryItemHit = true;

				if (HUD && HUD2) {
					HUD->SetVisibility(ESlateVisibility::Hidden);
					HUD2->SetVisibility(ESlateVisibility::Visible);
				}
            }

			else if (component->GetClass() == UTokenHolder::StaticClass()) {
				_LastMeshFocused = Cast<UStaticMeshComponent>(component->GetOwner()->GetComponentByClass(
					UStaticMeshComponent::StaticClass()));

				_LastMeshFocused->SetRenderCustomDepth(true);
				_LastMeshFocused->SetCustomDepthStencilValue(254);
				bInventoryItemHit = true;

				if (HUD && HUD2) {
					HUD->SetVisibility(ESlateVisibility::Hidden);
					HUD2->SetVisibility(ESlateVisibility::Visible);
				}
			}else{
				if (HUD && HUD2) {
					HUD2->SetVisibility(ESlateVisibility::Hidden);
					HUD->SetVisibility(ESlateVisibility::Visible);
				}
			}
        }
    }else{
		if (HUD && HUD2) {
			HUD2->SetVisibility(ESlateVisibility::Hidden);
			HUD->SetVisibility(ESlateVisibility::Visible);
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
		_LastPressed = _HitResult;
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
    if (_HitResult.GetActor() && _HitResult.GetActor() == _LastPressed.GetActor()) {
        TArray<UActorComponent*> Components;
        _HitResult.GetActor()->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                SERVER_UseReleased(Component);
            }
        }
    }
	// Si dejas de mirar al actor que usa UseSwitcher sin soltar el boton de Use,
	// en el momento de soltarlo automaticamente se libera para el anterior actor.
	else if (_LastPressed.GetActor()) {
		TArray<UActorComponent*> Components;
		_LastPressed.GetActor()->GetComponents(Components);

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

			if (_isTutorialEnabled) {
				APlayerController* PlayerController = Cast<APlayerController>(GetController());
				_Tutorial->Next(PlayerController, 6, false);//Next widget of tutorial
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

		if (_isTutorialEnabled) {
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			_Tutorial->Next(PlayerController, 6, false);//Next widget of tutorial
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
void AFPCharacter::TakeDropRight_Respawn(AActor* actor) {

	Super::TakeDropRight_Respawn(actor);

	if (actor->GetComponentByClass(UInventoryItem::StaticClass())) {

		if (_isTutorialEnabled) {
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			_Tutorial->Next(PlayerController, 2, false);//Next widget of tutorial

			_Tutorial->Last(PlayerController, 7, true, this);//Last widget of tutorial
		}

		/* Save scenary inventory item */
		SERVER_SaveItemInventory(actor, 0);
	}
	else if (actor->GetComponentByClass(UTokenHolder::StaticClass()) && _ItemRight &&
		_ItemRight->GetComponentByClass(UToken::StaticClass())) {
		SERVER_Drop(_ItemRight, 2);
		GrabbingRight = false;
	}
	else if (actor->GetComponentByClass(UHandPickItem::StaticClass())) {
		if (_ItemRight && _ItemRight->GetComponentByClass(UHandPickItem::StaticClass())) {
			/* Replace item */
			SERVER_Drop(_ItemRight, 2);
			SERVER_TakeRight(actor);
			UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(actor->GetComponentByClass(
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
			SERVER_TakeRight(actor);
			UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(actor->GetComponentByClass(
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
void AFPCharacter::TakeDropRight() {
    AActor* ActorFocused = GetItemFocused();
    if (ActorFocused) {
        if (ActorFocused->GetComponentByClass(UInventoryItem::StaticClass())) {

			if (_isTutorialEnabled) {
				APlayerController* PlayerController = Cast<APlayerController>(GetController());
				_Tutorial->Next(PlayerController, 2, false);//Next widget of tutorial

				_Tutorial->Last(PlayerController, 7, true, this);//Last widget of tutorial
			}

            /* Save scenary inventory item */
            SERVER_SaveItemInventory(ActorFocused, 0);
        }
		else if (ActorFocused->GetComponentByClass(UTokenHolder::StaticClass()) && _ItemRight &&
			_ItemRight->GetComponentByClass(UToken::StaticClass())) {
			SERVER_Drop(_ItemRight, 2);
			GrabbingRight = false;
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
                SERVER_SaveItemInventory(_ItemRight, 2); // AQUI CREO QUE HAY UN BUG, DEBERIA PULSAR DOS VECES PARA RECOGER UN OBJETO CON ESAS CARACTERISTICAS
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

		if (_isTutorialEnabled) {
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			_Tutorial->Next(PlayerController, 2, false);//Next widget of tutorial

			_Tutorial->Last(PlayerController, 7, true, this);//Last widget of tutorial
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

			if (_isTutorialEnabled) {
				APlayerController* PlayerController = Cast<APlayerController>(GetController());
				_Tutorial->Next(PlayerController, 2, false);//Next widget of tutorial

				_Tutorial->Last(PlayerController, 7, true, this);//Last widget of tutorial
			}

            /* Save scenary inventory item */
            SERVER_SaveItemInventory(ActorFocused, 0);
        }
		else if (ActorFocused->GetComponentByClass(UTokenHolder::StaticClass()) && _ItemLeft &&
			_ItemLeft->GetComponentByClass(UToken::StaticClass())) {
			SERVER_Drop(_ItemLeft, 1);
			GrabbingLeft = false;
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

		if (_isTutorialEnabled) {
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			_Tutorial->Next(PlayerController, 2, false);//Next widget of tutorial

			_Tutorial->Last(PlayerController, 7, true, this);//Last widget of tutorial
		}

        /* Save hand inventory item */
        SERVER_SaveItemInventory(_ItemLeft, 1);
    }
}

/********** DROP HAND ***********/
bool AFPCharacter::SERVER_Drop_Validate(AActor* ItemActor, int Hand) { return true; }
void AFPCharacter::SERVER_Drop_Implementation(AActor* ItemActor, int Hand) {
	CLIENT_ClearRadioDelegates(ItemActor);
	MULTI_Drop(ItemActor, Hand);
}
void AFPCharacter::MULTI_Drop_Implementation(AActor* ItemActor, int Hand) {
	UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ItemActor->GetComponentByClass(
		UStaticMeshComponent::StaticClass()));

	if (ItemMesh && ItemActor->GetComponentByClass(UToken::StaticClass())) {
		AActor* ActorFocused = GetItemFocused();
		if (ActorFocused) {
			UTokenHolder* Holder = Cast<UTokenHolder>(ActorFocused->GetComponentByClass(UTokenHolder::StaticClass()));

			ItemMesh->SetMobility(EComponentMobility::Movable);
			ItemActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			ItemActor->AttachToActor(ActorFocused, FAttachmentTransformRules::KeepRelativeTransform, TEXT("TablillaSocket"));

			_Inventory->RemoveItem(ItemActor);
			Holder->_Tablilla = ItemActor;
		}
	}
	
	else if (ItemMesh) {
		ItemMesh->SetMobility(EComponentMobility::Movable);
		ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		ItemMesh->SetSimulatePhysics(true);

		ItemActor->SetActorEnableCollision(true);

	}
	if (Hand == 1) _ItemLeft = nullptr;
	else if (Hand == 2) _ItemRight = nullptr;


	//Soltar objetos tras respawn de muerte
	if (Hand == 4) {
		_Inventory->RemoveItem(ItemActor);
		_ItemLeft = nullptr;
		_ItemRight = nullptr;
	}
}

/**************** TRIGGER INVENTORY *************/
/******HIDE INVENTORY FROM GAME MODE*****/
void AFPCharacter::HideInventory() {
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && _InventoryWidget) {
		if (!_IsInventoryHidden) {
			_InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
			_IsInventoryHidden = !_IsInventoryHidden;
		}
	}
}
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

				if (_isTutorialEnabled) {
					_Tutorial->Next(PlayerController, 3, false);//Next widget of tutorial
				}
            }
            else {
                _InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
                PlayerController->bShowMouseCursor = false;
                PlayerController->bEnableClickEvents = false;
                PlayerController->bEnableMouseOverEvents = false;
                PlayerController->SetInputMode(FInputModeGameOnly());

				if (_isTutorialEnabled) {
					_Tutorial->Next(PlayerController, 5, false);//Next widget of tutorial
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
			ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			_Inventory->AddItem(ItemActor);

			if (Hand == 1) { _ItemLeft = nullptr; GrabbingLeft = false; }
			else if (Hand == 2) { _ItemRight = nullptr; GrabbingRight = false; }
        }
    }
}

void AFPCharacter::PickItemInventory(AActor* ItemActor, FKey KeyStruct) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("PICK ITEM INVENTORY ")));
    if (ItemActor) {
        if (KeyStruct == EKeys::LeftMouseButton) {
            if (_ItemLeft && _ItemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {

				if (_isTutorialEnabled) {
					APlayerController* PlayerController = Cast<APlayerController>(GetController());
					_Tutorial->Next(PlayerController, 4, false);//Next widget of tutorial
				}

                /* Save hand inventory item */
                SERVER_SaveItemInventory(_ItemLeft, 1);
            }
            else if (_ItemLeft && _ItemLeft->GetComponentByClass(UHandPickItem::StaticClass())) {
                /* Drop item */
				SERVER_Drop(_ItemLeft, 1);
            }

			////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("left click EN ITEM INVENTORY ")));
            SERVER_PickItemInventoryLeft(ItemActor);
			if (_isTutorialEnabled) {
				APlayerController* PlayerController = Cast<APlayerController>(GetController());
				_Tutorial->Next(PlayerController, 4, false);//Next widget of tutorial
			}
        }
        else if (KeyStruct == EKeys::RightMouseButton) {
            if (_ItemRight && _ItemRight->GetComponentByClass(UInventoryItem::StaticClass())) {

				if (_isTutorialEnabled) {
					APlayerController* PlayerController = Cast<APlayerController>(GetController());
					_Tutorial->Next(PlayerController, 4, false);//Next widget of tutorial
				}
                /* Save hand inventory item */
                SERVER_SaveItemInventory(_ItemRight, 2);
            }
            else if (_ItemRight && _ItemRight->GetComponentByClass(UHandPickItem::StaticClass())) {
                /* Drop item */
                SERVER_Drop(_ItemRight, 2);
            }
			////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("RIGHT click EN ITEM INVENTORY ")));
            SERVER_PickItemInventoryRight(ItemActor);
			if (_isTutorialEnabled) {
				APlayerController* PlayerController = Cast<APlayerController>(GetController());
				_Tutorial->Next(PlayerController, 4, false);//Next widget of tutorial
			}
        }
    }
}

bool AFPCharacter::SERVER_PickItemInventoryLeft_Validate(AActor* ItemActor) { return true; }
void AFPCharacter::SERVER_PickItemInventoryLeft_Implementation(AActor* ItemActor) {
    CLIENT_AddRadioDelegates(ItemActor);
    MULTI_PickItemInventoryLeft(ItemActor);
}
void AFPCharacter::MULTI_PickItemInventoryLeft_Implementation(AActor* ItemActor) {
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
			/*
			ItemMesh->AttachToComponent(GetMesh(),
										FAttachmentTransformRules::KeepRelativeTransform,
										TEXT("itemHand_l"));

			ItemMesh->RelativeLocation = InventoryItemComp->_locationAttachFromInventory_L;
			ItemMesh->RelativeRotation = InventoryItemComp->_rotationAttachFromInventory_L;
			*/

			const UStaticMeshSocket* _GripSocket = ItemMesh->GetSocketByName(TEXT("Grip_L"));
			ItemMesh->AttachToComponent(_FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform,
				TEXT("GripPoint_L"));

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
}

bool AFPCharacter::SERVER_PickItemInventoryRight_Validate(AActor* ItemActor) { return true; }
void AFPCharacter::SERVER_PickItemInventoryRight_Implementation(AActor* ItemActor) {
    CLIENT_AddRadioDelegates(ItemActor);
    MULTI_PickItemInventoryRight(ItemActor);
}

void AFPCharacter::MULTI_PickItemInventoryRight_Implementation(AActor* ItemActor) {

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
			/*
			ItemMesh->AttachToComponent(GetMesh(),
										FAttachmentTransformRules::KeepRelativeTransform,
										TEXT("itemHand_r"));

			ItemMesh->RelativeLocation = InventoryItemComp->_locationAttachFromInventory_R;
			ItemMesh->RelativeRotation = InventoryItemComp->_rotationAttachFromInventory_R;
			*/
			const UStaticMeshSocket* _GripSocket = ItemMesh->GetSocketByName(TEXT("Grip_R"));
			ItemMesh->AttachToComponent(_FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform,
				TEXT("GripPoint_R"));
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
}

/****************************************** AUXILIAR FUNCTIONS ***********************************/
void AFPCharacter::ToggleMenuInteraction(bool Activate) {
    if (!_IsInventoryHidden) ToggleInventory();

    Super::ToggleMenuInteraction(Activate);
}

UTexture2D* AFPCharacter::GetItemTextureAt(int itemIndex) {
    return _Inventory->GetItemTextureAt(itemIndex);
}

AActor* AFPCharacter::GetItemFocused() {
    AActor* ActorFocused = _HitResult.GetActor();
    if (ActorFocused && ActorFocused->GetComponentByClass(UStaticMeshComponent::StaticClass()) &&
        (ActorFocused->GetComponentByClass(UInventoryItem::StaticClass()) ||
         ActorFocused->GetComponentByClass(UHandPickItem::StaticClass()) ||
	     ActorFocused->GetComponentByClass(UTokenHolder::StaticClass()))) {
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