// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "PlayerCharacter.h"

#include "Inventory.h"
#include "ItfUsable.h"
#include "ItfSwitcheable.h"
#include "ItfUsableItem.h"
#include "InventoryItem.h"
#include "HandPickItem.h"
#include "FMODAudioComponent.h"
#include "GameModePlay.h"
#include "Walkie.h"
#include "Components/WidgetInteractionComponent.h"

APlayerCharacter::APlayerCharacter() {
    bReplicates = true;
    bReplicateMovement = true;

    //set our turn rates for input
    _baseTurnRate = 45.f;
    _baseLookUpRate = 45.f;
    _ItemLeft = nullptr;
    _ItemRight = nullptr;
    _Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));

    _IsAction = false;

    /*RAYCAST PARAMETERS*/
    RayParameter = 250.0f;

    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    _PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
    _PlayerCamera->bUsePawnControlRotation = true;
    _PlayerCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("FPVCamera"));
    _WidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
    _WidgetInteractionComp->InteractionDistance = 1000000000;
    _WidgetInteractionComp->AttachToComponent(_PlayerCamera, FAttachmentTransformRules::KeepRelativeTransform);

    _StepsAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Audio"));

    _Health = 1;
}

void APlayerCharacter::BeginPlay() {
    Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);
    //_StepsAudioComp->SetParameter(FName("humedad"), 0.9);
	Raycasting();
}

FHitResult APlayerCharacter::Raycasting() {
    bool bHitRayCastFlag = false;
    FCollisionQueryParams CollisionInfo;

    FVector StartRaycast = _PlayerCamera->GetComponentLocation();
    FVector EndRaycast = _PlayerCamera->GetForwardVector() * RayParameter + StartRaycast;

    bHitRayCastFlag = GetWorld()->LineTraceSingleByChannel(hitResult, StartRaycast, EndRaycast, ECC_Visibility, CollisionInfo);
    //DrawDebugLine(GetWorld(), StartRaycast, EndRaycast, FColor(255, 0, 0), false, -1.0f, (uint8)'\000', 0.8f);

    if (bHitRayCastFlag && hitResult.Actor.IsValid()) {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *hitResult.Actor->GetName()));
        UActorComponent* actorComponent = hitResult.GetComponent();

        components = actorComponent->GetOwner()->GetComponentsByClass(UActorComponent::StaticClass());

        for (UActorComponent* component : components) {

            //Highlight outline colors:
            //GREEN: 252 | BLUE: 253 | ORANGE: 254 | WHITE: 255
            if (component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
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

    PlayerInput->BindAction("Use", IE_Pressed, this, &APlayerCharacter::UsePressed);
    PlayerInput->BindAction("Use", IE_Released, this, &APlayerCharacter::UseReleased);
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

/************** USE *************/
void APlayerCharacter::UsePressed() {
    /* RAYCASTING DETECTION */
    if (hitResult.GetActor()) {
        TArray<UActorComponent*> Components;
        hitResult.GetActor()->GetComponents(Components);

        for (UActorComponent* Component : Components) {
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
        TArray<UActorComponent*> Components;
        hitResult.GetActor()->GetComponents(Components);

        for (UActorComponent* Component : Components) {
            if (Component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                SERVER_UseReleased(Component);
            }
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
}

void APlayerCharacter::UseLeftReleased() {
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
}

/******* USE ITEM RIGHT *********/
void APlayerCharacter::UseRightPressed() {
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
}

void APlayerCharacter::UseRightReleased() {
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
}

/********** TAKE & DROP RIGHT HAND ***********/
void APlayerCharacter::TakeDropRight() {
    AActor* ActorFocused = GetItemFocused();
    if (ActorFocused) {
        if (ActorFocused->GetComponentByClass(UInventoryItem::StaticClass())) {
            /* Save scenary inventory item */
            SERVER_SaveItemInventory(ActorFocused);
        }
        else if(ActorFocused->GetComponentByClass(UHandPickItem::StaticClass())) {
            if (_ItemRight && _ItemRight->GetComponentByClass(UHandPickItem::StaticClass())) {
                /* Replace item */
                SERVER_DropRight();
                SERVER_TakeRight(ActorFocused);
            }
            else if (_ItemRight && _ItemRight->GetComponentByClass(UInventoryItem::StaticClass())) {
                /* Save hand inventory item */
                SERVER_SaveItemInventoryRight();
            }
            else if (!_ItemRight) {
                /* Take item */
                SERVER_TakeRight(ActorFocused);
            }
        }
    }
    else if (_ItemRight && _ItemRight->GetComponentByClass(UHandPickItem::StaticClass())) {
        /* Drop item */
        SERVER_DropRight();
    }
    else if (_ItemRight && _ItemRight->GetComponentByClass(UInventoryItem::StaticClass())) {
        /* Save hand inventory item */
        SERVER_SaveItemInventoryRight();
    }
}


/****TAKE RIGHT***/
bool APlayerCharacter::SERVER_TakeRight_Validate(AActor* Actor) { return true; }
void APlayerCharacter::SERVER_TakeRight_Implementation(AActor* Actor) {
    MULTI_TakeRight(Actor);
}

void APlayerCharacter::MULTI_TakeRight_Implementation(AActor* Actor) {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(
                                                            UStaticMeshComponent::StaticClass()));
    UHandPickItem* HandPickComp = Cast<UHandPickItem>(Actor->FindComponentByClass(
                                                            UHandPickItem::StaticClass()));
    if (ItemMesh && HandPickComp) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->SetSimulatePhysics(false);
        ItemMesh->AttachToComponent(GetMesh(),
                                FAttachmentTransformRules::KeepRelativeTransform,
                                TEXT("itemHand_r"));

        ItemMesh->RelativeLocation = HandPickComp->_locationAttach_R;
        ItemMesh->RelativeRotation = HandPickComp->_rotationAttach_R;

        Actor->SetActorEnableCollision(false);
        _ItemRight = Actor;
    }
}

bool APlayerCharacter::SERVER_DropRight_Validate() { return true; }
void APlayerCharacter::SERVER_DropRight_Implementation() {
    MULTI_DropRight();
}

void APlayerCharacter::MULTI_DropRight_Implementation() {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(_ItemRight->GetComponentByClass(
                                                            UStaticMeshComponent::StaticClass()));
    if (ItemMesh) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        ItemMesh->SetSimulatePhysics(true);
        
        _ItemRight->SetActorEnableCollision(true);
        _ItemRight = nullptr;
    }
}

/********** TAKE & DROP LEFT HAND ***********/
void APlayerCharacter::TakeDropLeft() {
    AActor* ActorFocused = GetItemFocused();
    if (ActorFocused) {
        if (ActorFocused->GetComponentByClass(UInventoryItem::StaticClass())) {
            /* Save scenary inventory item */
            SERVER_SaveItemInventory(ActorFocused);
        }
        else if (ActorFocused->GetComponentByClass(UHandPickItem::StaticClass())) {
            if (_ItemLeft && _ItemLeft->GetComponentByClass(UHandPickItem::StaticClass())) {
                /* Replace item */
                SERVER_DropLeft();
                SERVER_TakeLeft(ActorFocused);
            }
            else if (_ItemLeft && _ItemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {
                /* Save hand inventory item */
                SERVER_SaveItemInventoryLeft();
            }
            else if (!_ItemLeft) {
                /* Take item */
                SERVER_TakeLeft(ActorFocused);
            }
        }
    }
    else if (_ItemLeft && _ItemLeft->GetComponentByClass(UHandPickItem::StaticClass())) {
        /* Drop item */
        SERVER_DropLeft();
    }
    else if (_ItemLeft && _ItemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {
        /* Save hand inventory item */
        SERVER_SaveItemInventoryLeft();
    }
}


/****TAKE LEFT***/
bool APlayerCharacter::SERVER_TakeLeft_Validate(AActor* Actor) { return true; }
void APlayerCharacter::SERVER_TakeLeft_Implementation(AActor* Actor) {
    MULTI_TakeLeft(Actor);
}

void APlayerCharacter::MULTI_TakeLeft_Implementation(AActor* Actor) {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(
                                                            UStaticMeshComponent::StaticClass()));
    UHandPickItem* HandPickComp = Cast<UHandPickItem>(Actor->FindComponentByClass(
        UHandPickItem::StaticClass()));
    if (ItemMesh && HandPickComp) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->SetSimulatePhysics(false);
        ItemMesh->AttachToComponent(GetMesh(),
                                FAttachmentTransformRules::KeepRelativeTransform,
                                TEXT("itemHand_l"));

        ItemMesh->RelativeLocation = HandPickComp->_locationAttach_R;
        ItemMesh->RelativeRotation = HandPickComp->_rotationAttach_R;

        Actor->SetActorEnableCollision(false);
        _ItemLeft = Actor;
    }
}

bool APlayerCharacter::SERVER_DropLeft_Validate() { return true; }
void APlayerCharacter::SERVER_DropLeft_Implementation() {
    MULTI_DropLeft();
}

void APlayerCharacter::MULTI_DropLeft_Implementation() {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(_ItemLeft->GetComponentByClass(
                                                            UStaticMeshComponent::StaticClass()));
    if (ItemMesh) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        ItemMesh->SetSimulatePhysics(true);

        _ItemLeft->SetActorEnableCollision(true);
        _ItemLeft = nullptr;
    }
}

/**************************************** INVENTORY **********************************************/
bool APlayerCharacter::SERVER_SaveItemInventory_Validate(AActor* Actor) { return true; }
void APlayerCharacter::SERVER_SaveItemInventory_Implementation(AActor* Actor) {
    MULTI_SaveItemInventory(Actor);
}

void APlayerCharacter::MULTI_SaveItemInventory_Implementation(AActor* Actor) {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(
                                                            UStaticMeshComponent::StaticClass()));
    if (ItemMesh) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        _Inventory->AddItem(Actor);
    }
}

bool APlayerCharacter::SERVER_SaveItemInventoryLeft_Validate() { return true; }
void APlayerCharacter::SERVER_SaveItemInventoryLeft_Implementation() {
    MULTI_SaveItemInventoryLeft();
}

void APlayerCharacter::MULTI_SaveItemInventoryLeft_Implementation() {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(_ItemLeft->GetComponentByClass(
                                                            UStaticMeshComponent::StaticClass()));
    if (ItemMesh) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        _Inventory->AddItem(_ItemLeft);
        _ItemLeft = nullptr;
    }
}

bool APlayerCharacter::SERVER_SaveItemInventoryRight_Validate() { return true; }
void APlayerCharacter::SERVER_SaveItemInventoryRight_Implementation() {
    MULTI_SaveItemInventoryRight();
}

void APlayerCharacter::MULTI_SaveItemInventoryRight_Implementation() {
    UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(_ItemRight->GetComponentByClass(
                                                            UStaticMeshComponent::StaticClass()));
    if (ItemMesh) {
        ItemMesh->SetMobility(EComponentMobility::Movable);
        _Inventory->AddItem(_ItemRight);
        _ItemRight = nullptr;
    }
}

void APlayerCharacter::PickItemInventory(AActor* ItemActor, FKey KeyStruct) {
    SERVER_PickItemInventory(ItemActor, KeyStruct);
}

bool APlayerCharacter::SERVER_PickItemInventory_Validate(AActor* ItemActor, FKey KeyStruct) { return true; }
void APlayerCharacter::SERVER_PickItemInventory_Implementation(AActor* ItemActor, FKey KeyStruct) {
    MULTI_PickItemInventory(ItemActor, KeyStruct);
}

void APlayerCharacter::MULTI_PickItemInventory_Implementation(AActor* ItemActor, FKey KeyStruct) {
    /*Obtaining item components*/
    if (ItemActor) {
        UStaticMeshComponent* ItemMesh = Cast<UStaticMeshComponent>(ItemActor->GetComponentByClass(
                                                                UStaticMeshComponent::StaticClass()));
        UInventoryItem* InventoryItemComp = Cast<UInventoryItem>(ItemActor->GetComponentByClass(
                                                                 UInventoryItem::StaticClass()));
        if (ItemMesh && InventoryItemComp) {
            ItemMesh->SetMobility(EComponentMobility::Movable);
            ItemMesh->SetSimulatePhysics(false);

            if (KeyStruct == EKeys::LeftMouseButton) {
                if (_ItemLeft && _ItemLeft->GetComponentByClass(UInventoryItem::StaticClass())) {
                    /* Save hand inventory item */
                    SERVER_SaveItemInventoryLeft();
                }
                else if (_ItemLeft && _ItemLeft->GetComponentByClass(UHandPickItem::StaticClass())) {
                    /* Drop item */
                    SERVER_DropLeft();
                }
                ItemMesh->AttachToComponent(GetMesh(),
                                        FAttachmentTransformRules::KeepRelativeTransform,
                                        TEXT("itemHand_l"));

                ItemMesh->RelativeLocation = InventoryItemComp->_locationAttachFromInventory_L;
                ItemMesh->RelativeRotation = InventoryItemComp->_rotationAttachFromInventory_L;
                ItemMesh->GetOwner()->SetActorHiddenInGame(false);

                _ItemLeft = ItemActor;

                /*If the item is equipped in the other hand*/
                if (_ItemRight && _ItemRight == ItemActor) {
                    _ItemRight = nullptr;
                }
            }
            else if (KeyStruct == EKeys::RightMouseButton) {
                if (_ItemRight && _ItemRight->GetComponentByClass(UInventoryItem::StaticClass())) {
                    /* Save hand inventory item */
                    SERVER_SaveItemInventoryRight();
                }
                else if (_ItemRight && _ItemRight->GetComponentByClass(UHandPickItem::StaticClass())) {
                    /* Drop item */
                    SERVER_DropRight();
                }
                ItemMesh->AttachToComponent(GetMesh(),
                                        FAttachmentTransformRules::KeepRelativeTransform,
                                        TEXT("itemHand_r"));

                ItemMesh->RelativeLocation = InventoryItemComp->_locationAttachFromInventory_L;
                ItemMesh->RelativeRotation = InventoryItemComp->_rotationAttachFromInventory_L;
                ItemMesh->GetOwner()->SetActorHiddenInGame(false);

                _ItemRight = ItemActor;

                /*If the item is equipped in the other hand*/
                if (_ItemLeft && _ItemLeft == ItemActor) {
                    _ItemLeft = nullptr;
                }
            }
        }
    }
}


/****************************************** AUXILIAR FUNCTIONS ***********************************/
/******* Radio Delegate *******/
void APlayerCharacter::AddRadioDelegates(AActor* Actor) {
    UWalkie* Walkie = Cast<UWalkie>(Actor->GetComponentByClass(UWalkie::StaticClass()));
    if (Walkie && !Walkie->_AreDelegatesBinded) {
        _OnRadioPressedDelegateHandle = Walkie->AddOnRadioDelegate(_OnRadioPressedDelegate, true);
        _OnRadioReleasedDelegateHandle = Walkie->AddOnRadioDelegate(_OnRadioReleasedDelegate, false);

        const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
        FString myRole = EnumPtr->GetEnumName((int32)Role);
        ULibraryUtils::Log(FString::Printf(TEXT("AddRadioDelegates : %s"), *myRole), 0, 60);
    }
}

void APlayerCharacter::ClearRadioDelegates(AActor* Actor) {
    UWalkie* Walkie = Cast<UWalkie>(Actor->GetComponentByClass(UWalkie::StaticClass()));
    if (Walkie && Walkie->_AreDelegatesBinded) {
        Walkie->ClearOnRadioDelegate(_OnRadioPressedDelegateHandle, true);
        Walkie->ClearOnRadioDelegate(_OnRadioReleasedDelegateHandle, false);

        const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
        FString myRole = EnumPtr->GetEnumName((int32)Role);
        ULibraryUtils::Log(FString::Printf(TEXT("AddRadioDelegates : %s"), *myRole), 0, 60);
    }
}

void APlayerCharacter::SetHUDVisible(bool visible) {
    _isVisible = visible;
}

bool APlayerCharacter::IsHUDVisible() {
    return _isVisible;
}

UInventory* APlayerCharacter::GetInventory() {
    return _Inventory;
}

UTexture2D* APlayerCharacter::GetItemAt(int itemIndex) {
    return _Inventory->GetItemAt(itemIndex);
}

AActor* APlayerCharacter::GetItemFocused() {
    AActor* ActorFocused = hitResult.GetActor();
    if(ActorFocused && ActorFocused->GetComponentByClass(UStaticMeshComponent::StaticClass()) &&
                            (ActorFocused->GetComponentByClass(UInventoryItem::StaticClass()) ||
                             ActorFocused->GetComponentByClass(UHandPickItem::StaticClass()))) {
        return ActorFocused;
    }
    return nullptr;
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
