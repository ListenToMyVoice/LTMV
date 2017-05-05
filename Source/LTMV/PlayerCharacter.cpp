// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "PlayerCharacter.h"

#include "Projectile.h"
#include "Gun.h"
#include "Inventory.h"
#include "ItfUsable.h"
#include "ItfSwitcheable.h"
#include "ItfUsableItem.h"
#include "InventoryItem.h"
#include "HandPickItem.h"
#include "FMODAudioComponent.h"
#include "GameModePlay.h"
#include "Walkie.h"
#include "FMODEvent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Components/WidgetInteractionComponent.h"

APlayerCharacter::APlayerCharacter() {
    bReplicates = true;
    bReplicateMovement = true;

    //set our turn rates for input
    _baseTurnRate = 45.f;
    _baseLookUpRate = 45.f;
    _itemLeft = nullptr;
    _itemRight = nullptr;
    _inventory = nullptr;

    _isAction = false;

    /*RAYCAST PARAMETERS*/
    RayParameter = 250.0f;

    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    _PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
    _PlayerCamera->bUsePawnControlRotation = true;
    _PlayerCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("FPVCamera"));
    _WidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
    _WidgetInteractionComp->InteractionDistance = 1000000000;
    _WidgetInteractionComp->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform);
	_AlternateMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AlternateMesh"));
	_AlternateMesh->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform);
	_AlternateMesh->SetHiddenInGame(true);

    _StepsAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Audio"));

	OnActorHit.AddDynamic(this, &APlayerCharacter::OnHit);
    _Health = 1;

    //static ConstructorHelpers::FClassFinder<UInventoryWidget> InventoryWidgetClassFinder(TEXT(
    //    "/Game/BluePrints/HUD/InventoryHUD"));
    //InventoryUIClass = InventoryWidgetClassFinder.Class;
}

void APlayerCharacter::BeginPlay() {
    Super::BeginPlay();
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Hearing::StaticClass(), this);
}

void APlayerCharacter::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);
    //_StepsAudioComp->SetParameter(FName("humedad"), 0.9);
	Raycasting();
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInput) {
    check(PlayerInput);
	
    /* MOVEMENT */
    PlayerInput->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInput->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInput->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInput->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    PlayerInput->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInput->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
    PlayerInput->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInput->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

    /* SERVER */
    PlayerInput->BindAction("TakeDropRight", IE_Released, this, &APlayerCharacter::TakeDropRight);
    PlayerInput->BindAction("TakeDropLeft", IE_Released, this, &APlayerCharacter::TakeDropLeft);
    PlayerInput->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::execOnStartCrouching);
    PlayerInput->BindAction("Crouch", IE_Released, this, &APlayerCharacter::execOnEndCrouching);

    PlayerInput->BindAction("Use", IE_Pressed, this, &APlayerCharacter::UsePressed);
    PlayerInput->BindAction("Use", IE_Released, this, &APlayerCharacter::UseReleased);
}

FHitResult APlayerCharacter::Raycasting() {
    bool bHitRayCastFlag = false;
    FCollisionQueryParams CollisionInfo;

    FVector StartRaycast = _PlayerCamera->GetComponentLocation();
    FVector EndRaycast = _PlayerCamera->GetForwardVector() * RayParameter + StartRaycast;

    bHitRayCastFlag = GetWorld()->LineTraceSingleByChannel(hitResult, StartRaycast, EndRaycast, ECC_Visibility, CollisionInfo);
    //DrawDebugLine(GetWorld(), StartRaycast, EndRaycast, FColor(255, 0, 0), false, -1.0f, (uint8)'\000', 0.8f);

    if (bHitRayCastFlag && hitResult.Actor.IsValid()) {

        _itemFocused = ItemFocused();

        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *hitResult.Actor->GetName()));
        UActorComponent* actorComponent = hitResult.GetComponent();

        components = actorComponent->GetOwner()->GetComponentsByClass(UActorComponent::StaticClass());

            for (UActorComponent* component : components) {

                //Highlight outline colors:
                //GREEN: 252 | BLUE: 253 | ORANGE: 254 | WHITE: 255
                if (component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())){
                    lastMeshFocused = Cast<UStaticMeshComponent>(component->GetOwner()->GetComponentByClass(
                        UStaticMeshComponent::StaticClass()));

                    lastMeshFocused->SetRenderCustomDepth(true);
                    lastMeshFocused->SetCustomDepthStencilValue(252);
                    bInventoryItemHit = true;
                }
                else if (component->GetClass() == UInventoryItem::StaticClass()) {
                    lastMeshFocused = Cast<UStaticMeshComponent>(component->GetOwner()->GetComponentByClass(
                        UStaticMeshComponent::StaticClass()));

                    lastMeshFocused->SetRenderCustomDepth(true);
                    lastMeshFocused->SetCustomDepthStencilValue(253);
                    bInventoryItemHit = true;
                }
                else if (component->GetClass() == UHandPickItem::StaticClass()) {
                    lastMeshFocused = Cast<UStaticMeshComponent>(component->GetOwner()->GetComponentByClass(
                        UStaticMeshComponent::StaticClass()));

                    lastMeshFocused->SetRenderCustomDepth(true);
                    lastMeshFocused->SetCustomDepthStencilValue(255);
                    bInventoryItemHit = true;
                }

                //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *hitResult.Actor->GetName()));
        }
    }
    
    //If Raycast is not hitting any actor, disable the outline
    if (bInventoryItemHit && hitResult.Actor != lastMeshFocused->GetOwner()) {
      
        lastMeshFocused->SetCustomDepthStencilValue(0);
        lastMeshFocused->SetRenderCustomDepth(false);

        bInventoryItemHit = false;
    }
    

    return hitResult;
}

/****************************************** ACTION MAPPINGS **************************************/
/*********** MOVEMENT ***********/
void APlayerCharacter::MoveForward(float Value) {
    if (Value != 0.0f) {
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void APlayerCharacter::MoveRight(float Value) {
    if (Value != 0.0f) {
        AddMovementInput(GetActorRightVector(), Value);
    }
}

void APlayerCharacter::TurnAtRate(float Rate) {
    AddControllerYawInput(Rate * _baseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate) {
    AddControllerPitchInput(Rate * _baseLookUpRate * GetWorld()->GetDeltaSeconds());
}

/*********** MOVEMENT ***********/

void APlayerCharacter::execOnStartCrouching()
{
    OnStartCrouch(0,0);
}

void APlayerCharacter::execOnEndCrouching()
{
    OnEndCrouch(0, 0);
}

/************** USE *************/
void APlayerCharacter::UsePressed() {
    /* RAYCASTING DETECTION */
    if (hitResult.GetActor()) {
        TArray<UActorComponent*> Components;
        hitResult.GetActor()->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Component: "), *Component->GetName()));
            if (Component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                SERVER_UsePressed(Component);
            }
        }
    }
}


bool APlayerCharacter::SERVER_UsePressed_Validate(UActorComponent* component) { return true; }
void APlayerCharacter::SERVER_UsePressed_Implementation(UActorComponent* component) {
    MULTI_UsePressed(component);
}

void APlayerCharacter::MULTI_UsePressed_Implementation(UActorComponent* component) {
    IItfUsable* itfObject = Cast<IItfUsable>(component);;
    if (itfObject) itfObject->Execute_UsePressed(component);
}

void APlayerCharacter::UseReleased() {
    /* RAYCASTING DETECTION */
    if (hitResult.GetActor()) {
        UActorComponent* component = hitResult.GetActor()->GetComponentByClass(UActorComponent::StaticClass());
        if (component && component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
            SERVER_UseReleased(component);
        }
    }
}
bool APlayerCharacter::SERVER_UseReleased_Validate(UActorComponent* component) { return true; }
void APlayerCharacter::SERVER_UseReleased_Implementation(UActorComponent* component) {
    MULTI_UseReleased(component);
}

void APlayerCharacter::MULTI_UseReleased_Implementation(UActorComponent* component) {
    IItfUsable* itfObject = Cast<IItfUsable>(component);
    if (itfObject) itfObject->Execute_UseReleased(component);
}

/******** USE ITEM LEFT *********/
void APlayerCharacter::UseLeftPressed() {
    if (_itemLeft) {
        TArray<UActorComponent*> Components;
        _itemLeft->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsableItem::StaticClass())) {
                IItfUsableItem* ItfObject = Cast<IItfUsableItem>(Component);
                if (ItfObject) ItfObject->Execute_UseItemPressed(Component);
            }
        }
    }
}

void APlayerCharacter::UseLeftReleased() {
    if (_itemLeft) {
        TArray<UActorComponent*> Components;
        _itemLeft->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsableItem::StaticClass())) {
                IItfUsableItem* ItfObject = Cast<IItfUsableItem>(Component);
                if (ItfObject) ItfObject->Execute_UseItemReleased(Component);
            }
        }
    }
}

/******* USE ITEM RIGHT *********/
void APlayerCharacter::UseRightPressed() {
    if (_itemRight) {
        TArray<UActorComponent*> Components;
        _itemRight->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsableItem::StaticClass())) {
                IItfUsableItem* ItfObject = Cast<IItfUsableItem>(Component);
                if (ItfObject) ItfObject->Execute_UseItemPressed(Component);
            }
        }
    }
}

void APlayerCharacter::UseRightReleased() {
    if (_itemRight) {
        TArray<UActorComponent*> Components;
        _itemRight->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsableItem::StaticClass())) {
                IItfUsableItem* ItfObject = Cast<IItfUsableItem>(Component);
                if (ItfObject) ItfObject->Execute_UseItemReleased(Component);
            }
        }
    }
}

/********** TAKE & DROP RIGHT HAND ***********/
//Cambiar
void APlayerCharacter::TakeDropRight() {
	

    if (ItemFocused()) {
        UActorComponent* takeComp = hitResult.GetActor()->GetComponentByClass(
            UActorComponent::StaticClass());

        if (!_itemRight) {
            _itemRight = hitResult.GetActor();
            if(takeComp)
                SERVER_TakeDropRight(_itemRight);
        }

        else {
            /*REPLACE LEFT HANDPICKITEM ONLY*/
            if (_itemRight && _itemRight->GetComponentByClass(UHandPickItem::StaticClass())) {
                SERVER_DropRight();

                _itemRight = hitResult.GetActor();
                if (takeComp)
                    SERVER_TakeDropRight(_itemRight);
            }
            else if (_itemRight && _itemRight->GetComponentByClass(UInventoryItem::StaticClass())) {
                if (takeComp)
                    SERVER_TakeDropRight(_itemRight);
            }
        }
    }

    else {
        /*DROP RIGHT ITEM*/
        if (_itemRight && _itemRight->GetComponentByClass(UHandPickItem::StaticClass())) {
            SERVER_DropRight();
        }
        /*SAVE ITEM FROM HAND TO INVENTORY*/
        if (_itemRight && _itemRight->GetComponentByClass(UInventoryItem::StaticClass())) {
            SaveInventory(_itemRight);
            /*Coger libraryutils*/
            UE_LOG(LogTemp, Warning, TEXT("SAVED ITEM: %s"), *_itemRight->GetName());
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SAVED ITEM: %s"), *_itemRight->GetName()));
            Cast<UInventoryItem>(_itemRight->GetComponentByClass(
                UInventoryItem::StaticClass()))->SetEquipped(false);
            _itemRight = nullptr;
        }
    }

}


/****TAKE RIGHT***/
bool APlayerCharacter::SERVER_TakeDropRight_Validate(AActor* actor) { return true; }
void APlayerCharacter::SERVER_TakeDropRight_Implementation(AActor* actor) {
    MULTI_TakeDropRight(actor);
}

void APlayerCharacter::MULTI_TakeDropRight_Implementation(AActor* actor) {
    _itemRight = actor;

    UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(_itemRight->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));

    /****GUESS TAKECOMP***/
    if (mesh && _itemRight->FindComponentByClass(UInventoryItem::StaticClass())) {

        mesh->SetMobility(EComponentMobility::Movable);
        SaveInventory(_itemRight);
        _itemRight = nullptr;
    }

    else if (mesh && _itemRight->FindComponentByClass(UHandPickItem::StaticClass())) {

        UHandPickItem* HandPickComp = Cast<UHandPickItem>(_itemRight->FindComponentByClass(
            UHandPickItem::StaticClass()));

        mesh->SetMobility(EComponentMobility::Movable);
        mesh->SetSimulatePhysics(false);

        mesh->AttachToComponent(GetMesh(),
            FAttachmentTransformRules::KeepRelativeTransform,
            TEXT("itemHand_r"));

        mesh->RelativeLocation = HandPickComp->_locationAttach_R;
        mesh->RelativeRotation = HandPickComp->_rotationAttach_R;

        _itemRight->SetActorEnableCollision(false);
    }
}

/********** TAKE & DROP LEFT HAND ***********/
//Cambiar
void APlayerCharacter::TakeDropLeft() {
    if (ItemFocused()) {
        UActorComponent* takeComp = hitResult.GetActor()->GetComponentByClass(
            UActorComponent::StaticClass());

        if (!_itemLeft) {
            _itemLeft = hitResult.GetActor();
            if(takeComp)
                SERVER_TakeDropLeft(_itemLeft);
        }

        else {
            /*REPLACE LEFT HANDPICKITEM ONLY*/
            if (_itemLeft && _itemLeft->GetComponentByClass(UHandPickItem::StaticClass())) {
                SERVER_DropLeft();

                _itemLeft = hitResult.GetActor();
                if (takeComp)
                    SERVER_TakeDropLeft(_itemLeft);
            }
            else if (_itemLeft && _itemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {
                if (takeComp)
                    SERVER_TakeDropLeft(_itemLeft);
            }
        }
    }

    else {
        /*DROP LEFT ITEM*/
        if (_itemLeft && _itemLeft->GetComponentByClass(UHandPickItem::StaticClass())) {
            SERVER_DropLeft();
        }
        /*SAVE ITEM FROM HAND TO INVENTORY*/
        if (_itemLeft && _itemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {
            SaveInventory(_itemLeft);
            UE_LOG(LogTemp, Warning, TEXT("SAVED ITEM: %s"), *_itemLeft->GetName());
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SAVED ITEM: %s"), *_itemLeft->GetName()));
            Cast<UInventoryItem>(_itemLeft->GetComponentByClass(
                UInventoryItem::StaticClass()))->SetEquipped(false);
            _itemLeft = nullptr;
        }
    }

}

/****TAKE LEFT***/
bool APlayerCharacter::SERVER_TakeDropLeft_Validate(AActor* actor) { return true; }
void APlayerCharacter::SERVER_TakeDropLeft_Implementation(AActor* actor) {
    MULTI_TakeDropLeft(actor);
}

void APlayerCharacter::MULTI_TakeDropLeft_Implementation(AActor* actor) {
    _itemLeft = actor;
 
    UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(_itemLeft->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));

    /****GUESS TAKECOMP***/
    if (mesh && _itemLeft->FindComponentByClass(UInventoryItem::StaticClass())) {
        mesh->SetMobility(EComponentMobility::Movable);
        SaveInventory(_itemLeft);
        _itemLeft = nullptr;
    }

    else if (mesh && _itemLeft->FindComponentByClass(UHandPickItem::StaticClass())) {
        UHandPickItem* HandPickComp = Cast<UHandPickItem>(_itemLeft->FindComponentByClass(
            UHandPickItem::StaticClass()));

        mesh->SetMobility(EComponentMobility::Movable);
        mesh->SetSimulatePhysics(false);

		if (_itemLeft->FindComponentByClass(UGun::StaticClass()) == false) {
			mesh->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::KeepRelativeTransform,
				TEXT("itemHand_l"));

			mesh->RelativeLocation = HandPickComp->_locationAttach_L;
			mesh->RelativeRotation = HandPickComp->_rotationAttach_L;
			ULibraryUtils::Log(FString::Printf(TEXT("Atachado a mano")), 0, 60);

		}

		if(_itemLeft->FindComponentByClass(UGun::StaticClass())) {
			ULibraryUtils::Log(FString::Printf(TEXT("WARNING: Terminar implementación ambas manos y volver a mostrar")),1, 60);
			_AlternateMesh->SetHiddenInGame(false);
			GetMesh()->SetHiddenInGame(true);
			UInventory* bag = Cast<UInventory>(GetComponentByClass(UInventory::StaticClass()));
			if (bag) { bag->SetHiddenInGame(true); }
			mesh->SetMobility(EComponentMobility::Movable);
			mesh->SetSimulatePhysics(false);

			mesh->AttachToComponent(_PlayerCamera,
				FAttachmentTransformRules::KeepRelativeTransform);
			mesh->RelativeLocation = HandPickComp->_locationAttach_C;
			mesh->RelativeRotation = HandPickComp->_rotationAttach_C;
			ULibraryUtils::Log(FString::Printf(TEXT("Atachado a camara")), 0, 60);
		}


        _itemLeft->SetActorEnableCollision(false);
    }
}

bool APlayerCharacter::SERVER_DropLeft_Validate() { return true; }
void APlayerCharacter::SERVER_DropLeft_Implementation() {
    MULTI_DropLeft();
}

void APlayerCharacter::MULTI_DropLeft_Implementation() {
    if (_itemLeft) {
        UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(_itemLeft->GetComponentByClass(
            UStaticMeshComponent::StaticClass()));
    
        if (mesh) {
            mesh->SetMobility(EComponentMobility::Movable);
            mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
            mesh->SetSimulatePhysics(true);
        }

        if (_itemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {
            UInventoryItem* inventoryItem = Cast<UInventoryItem>(_itemLeft->GetComponentByClass(
                UInventoryItem::StaticClass()));
            inventoryItem->SetEquipped(false);
            this->_inventory->RemoveItem(_itemLeft);

        }

        _itemLeft->SetActorEnableCollision(true);
        _itemLeft = nullptr;    
    
    }


}

bool APlayerCharacter::SERVER_DropRight_Validate() { return true; }
void APlayerCharacter::SERVER_DropRight_Implementation() {
    MULTI_DropRight();
}

void APlayerCharacter::MULTI_DropRight_Implementation() {

    if (_itemRight) {

        UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(_itemRight->GetComponentByClass(
            UStaticMeshComponent::StaticClass()));

        if (mesh) {
            mesh->SetMobility(EComponentMobility::Movable);
            mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
            mesh->SetSimulatePhysics(true);
        }

        if (_itemRight->GetComponentByClass(UInventoryItem::StaticClass())) {
            UInventoryItem* inventoryItem = Cast<UInventoryItem>(_itemRight->GetComponentByClass(
                UInventoryItem::StaticClass()));
            inventoryItem->SetEquipped(false);
            this->_inventory->RemoveItem(_itemRight);

        }

        _itemRight->SetActorEnableCollision(true);
        _itemRight = nullptr;
    }
    
}

/****************************************** ACTIONS **********************************************/
/*** SAVING ***/
void APlayerCharacter::SaveInventory(AActor* item) {
    this->_inventory = Cast<UInventory>(this->FindComponentByClass(UInventory::StaticClass()));
    if (_inventory) _inventory->AddItem(item);
}

void APlayerCharacter::SetHUDVisible(bool visible) {
    _isVisible = visible;
}

bool APlayerCharacter::IsHUDVisible() {
    return _isVisible;
}

UTexture2D* APlayerCharacter::GetItemAt(int itemIndex) {
    if (this->_inventory)
        return _inventory->GetItemAt(itemIndex);
    else return nullptr;
}

/*** TAKE ITEM FROM INVENTORY TO HAND ***/
void APlayerCharacter::PickItemFromInventory(AActor* ItemActor, FKey KeyStruct) {
    SERVER_PickItemFromInventory(ItemActor, KeyStruct);
}

bool APlayerCharacter::SERVER_PickItemFromInventory_Validate(AActor* ItemActor, FKey KeyStruct) { 
    return true;
}
void APlayerCharacter::SERVER_PickItemFromInventory_Implementation(AActor* ItemActor, FKey KeyStruct) {
    MULTI_PickItemFromInventory(ItemActor, KeyStruct);
}

void APlayerCharacter::MULTI_PickItemFromInventory_Implementation(AActor* ItemActor, FKey KeyStruct) {
    this->_inventory = Cast<UInventory>(this->FindComponentByClass(UInventory::StaticClass()));
    if (_inventory) {
        AActor* ItemFromInventory = nullptr;

        ItemFromInventory = _inventory->PickItem(ItemActor);

        UStaticMeshComponent* ItemMesh = nullptr;
        UInventoryItem* InventoryItemComponent = nullptr;

        /*Obtaining item components*/
        if (ItemFromInventory) {
            ItemMesh = Cast<UStaticMeshComponent>(ItemFromInventory->GetComponentByClass(
                UStaticMeshComponent::StaticClass()));

            InventoryItemComponent = Cast<UInventoryItem>(ItemFromInventory->GetComponentByClass(
                UInventoryItem::StaticClass()));
        }

        if (ItemMesh) {
            ItemMesh->SetMobility(EComponentMobility::Movable);
            ItemMesh->SetSimulatePhysics(false);
        }

        if (KeyStruct == EKeys::LeftMouseButton) {
            if (ItemMesh) {
                if (_itemLeft && _itemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {
                    SaveInventory(_itemLeft);
                    Cast<UInventoryItem>(_itemLeft->GetComponentByClass(
                        UInventoryItem::StaticClass()))->SetEquipped(false);
                    _itemLeft = nullptr;
                }
                if (_itemLeft && _itemLeft->GetComponentByClass(UHandPickItem::StaticClass())) {
                    SERVER_DropLeft();
                    return;
                }


                ItemMesh->AttachToComponent(GetMesh(),
                                            FAttachmentTransformRules::KeepRelativeTransform,
                                            TEXT("itemHand_l"));

                ItemMesh->RelativeLocation = InventoryItemComponent->_locationAttachFromInventory_L;
                ItemMesh->RelativeRotation = InventoryItemComponent->_rotationAttachFromInventory_L;
                ItemMesh->GetOwner()->SetActorHiddenInGame(false);

                InventoryItemComponent->SetEquipped(true);

                _itemLeft = ItemFromInventory;
                AddRadioDelegates(ItemFromInventory);

                /*If the item is equipped in the other hand*/
                if (_itemRight && _itemRight == ItemFromInventory) {
                    _itemRight = nullptr;
                }
            }
        }

        if (KeyStruct == EKeys::RightMouseButton) {
            if (ItemMesh) {
                if (_itemRight && _itemRight->GetComponentByClass(UInventoryItem::StaticClass())) {
                    SaveInventory(_itemRight);
                    Cast<UInventoryItem>(_itemRight->GetComponentByClass(
                        UInventoryItem::StaticClass()))->SetEquipped(false);
                    _itemRight = nullptr;
                }
                if (_itemRight && _itemRight->GetComponentByClass(UHandPickItem::StaticClass())) {
                    SERVER_DropRight();
                    return;
                }

                ItemMesh->AttachToComponent(GetMesh(),
                                            FAttachmentTransformRules::KeepRelativeTransform,
                                            TEXT("itemHand_r"));

                ItemMesh->RelativeLocation = InventoryItemComponent->_locationAttachFromInventory_R;
                ItemMesh->RelativeRotation = InventoryItemComponent->_rotationAttachFromInventory_R;
                ItemMesh->GetOwner()->SetActorHiddenInGame(false);

                InventoryItemComponent->SetEquipped(true);

                _itemRight = ItemFromInventory;
                AddRadioDelegates(ItemFromInventory);

                /*If the item is equipped in the other hand*/
                if (_itemLeft && _itemLeft == ItemFromInventory) {
                    _itemLeft = nullptr;
                }
            }
        }
    }
}

/****************************************** AUXILIAR FUNCTIONS ***********************************/
/* Radio Delegate */
void APlayerCharacter::AddRadioDelegates(AActor* PickedActor) {
    UWalkie* Walkie = Cast<UWalkie>(PickedActor->GetComponentByClass(UWalkie::StaticClass()));
    if (Walkie) {
        _OnRadioPressedDelegateHandle = Walkie->AddOnRadioDelegate(_OnRadioPressedDelegate, true);
        _OnRadioReleasedDelegateHandle = Walkie->AddOnRadioDelegate(_OnRadioReleasedDelegate, false);

        const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
        FString myRole = EnumPtr->GetEnumName((int32)Role);

        ULibraryUtils::Log(FString::Printf(TEXT("AddRadioDelegates : %s"), *myRole), 0, 60);
    }
}

bool APlayerCharacter::ItemFocused() {
    if(hitResult.GetActor())
        if (hitResult.GetActor()->GetComponentByClass(UInventoryItem::StaticClass())
            || hitResult.GetActor()->GetComponentByClass(UHandPickItem::StaticClass())) {
            return true;
        }

    return false;
}

UInventory* APlayerCharacter::GetInventory() {
    return this->_inventory;
}

/*****************AI****************************/
void APlayerCharacter::NoiseManager(UFMODEvent* FMODSoundEvent) {

	//if (_StepsAudioComp->Event == FMODSoundEvent) {
	UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 0.4, this);
	//}
}

/***********RECEIVE HIT AND DAMAGE*************/
void APlayerCharacter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor) {
		if (OtherActor->IsA(AProjectile::StaticClass())) {
			// Create a damage event  
			TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
			FDamageEvent DamageEvent(ValidDamageTypeClass);

			const float DamageAmount = 1.0f;
			TakeDamage(DamageAmount, DamageEvent, GetController(), OtherActor);
		}
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                   class AController* EventInstigator, class AActor* DamageCauser) {
    //Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    _Health -= DamageAmount;
    if (_Health <= 0) {
        AGameModePlay* GameMode = Cast<AGameModePlay>(GetWorld()->GetAuthGameMode());
        if (GameMode) {
            GameMode->SERVER_PlayerDead(GetController());
            MULTI_CharacterDead();
        }
    }
    return _Health;
}

void APlayerCharacter::MULTI_CharacterDead_Implementation() {
    Cast<UPrimitiveComponent>(GetRootComponent())->SetCollisionProfileName(FName("Ragdoll"));
    SetActorEnableCollision(true);
    GetMesh()->SetSimulatePhysics(true);
}
