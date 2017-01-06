// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "PlayerCharacter.h"

#include "ItemTakeLeft.h"
#include "ItemTakeRight.h"
#include "ItemSave.h"

APlayerCharacter::APlayerCharacter() {
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // set our turn rates for input
    _baseTurnRate = 45.f;
    _baseLookUpRate = 45.f;

    _itemLeft = nullptr;
    _itemRight = nullptr;

    _activeScenaryItems = {};
}

void APlayerCharacter::BeginPlay() {
    Super::BeginPlay();
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
    check(PlayerInputComponent);

    PlayerInputComponent->BindAction("TakeLeft", IE_Released, this, &APlayerCharacter::TakeLeft);
    PlayerInputComponent->BindAction("TakeRight", IE_Released, this, &APlayerCharacter::TakeRight);
    
    PlayerInputComponent->BindAction("SaveLeft", IE_Released, this, &APlayerCharacter::SaveLeft);
    PlayerInputComponent->BindAction("SaveRight", IE_Released, this, &APlayerCharacter::SaveRight);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

    PlayerInputComponent->BindAction("Help", IE_Released, this, &APlayerCharacter::Help);
    PlayerInputComponent->BindAction("Use", IE_Released, this, &APlayerCharacter::Use);
}

/****************************************** ACTION MAPPINGS **************************************/

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

void APlayerCharacter::TakeLeft() {
    if (_itemLeft && _activeScenaryItems.Num() > 0) {
        // REPLACE
        DropItemLeft();
        TakeItemLeft();
    }
    else if (_itemLeft && _activeScenaryItems.Num() <= 0) {
        // DROP
        DropItemLeft();
    }
    else if (!_itemLeft && _activeScenaryItems.Num() > 0) {
        // TAKE
        TakeItemLeft();
    }
}

void APlayerCharacter::TakeRight() {
    if (_itemRight && _activeScenaryItems.Num() > 0) {
        // REPLACE
        DropItemRight();
        TakeItemRight();
    }
    else if (_itemRight && _activeScenaryItems.Num() <= 0) {
        // DROP
        DropItemRight();
    }
    else if (!_itemRight && _activeScenaryItems.Num() > 0) {
        // TAKE
        TakeItemRight();
    }
}

void APlayerCharacter::SaveLeft() {
    if (_itemLeft) {
        // SAVE
        SaveInventory(_itemLeft);
        _itemLeft = nullptr;
    }
}

void APlayerCharacter::SaveRight() {
    if (_itemRight) {
        // SAVE
        SaveInventory(_itemRight);
        _itemRight = nullptr;
    }
}

void APlayerCharacter::Help() {
    ULibraryUtils::Log(TEXT("ACTIVE ITEMS:"), 3);
    for (AItemActor* itemActor : _activeScenaryItems) {
        ULibraryUtils::Log(itemActor->_name.ToString(), 3);
    }
}

void APlayerCharacter::Use() {
    const TSet <UActorComponent*> set = _activeScenaryItems[_activeScenaryItems.Num()-1]->
                                            GetComponents();

    UObject* pointerToAnyUObject = nullptr;
    for (UActorComponent* component : set) {
        if (component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
            IItfUsable* itfObject = Cast<IItfUsable>(component);
            if (itfObject) {
                itfObject->Execute_Use(pointerToAnyUObject);
            }
        }
    }


    //ItemData data = FindItemAndComponents(IItfUsable::StaticClass());
    ////UItemTakeRight* takeComp = data.actor ? Cast<IItfUsable>(data.components[0]) : nullptr;

    //for (UActorComponent* itemActor : data.components) {
    //    IItfUsable* useComp = Cast<IItfUsable>(data.components[0]);
    //    if (useComp) useComp->Execute_Use();
    //}
}

/****************************************** ACTIONS **********************************************/

/*** TAKING DROPING ***/
void APlayerCharacter::TakeItemLeft() {
    ItemData data = FindItemAndComponents(UItemTakeLeft::StaticClass());
    UItemTakeLeft* takeComp = data.actor ? Cast<UItemTakeLeft>(data.components[0]) : nullptr;

    if (takeComp) {
        _itemLeft = data.actor;
        UStaticMeshComponent* mesh = _itemLeft->GetStaticMeshComponent();
        if (mesh) {
            mesh->SetSimulatePhysics(false);

            mesh->AttachToComponent(GetMesh(),
                                    FAttachmentTransformRules::KeepRelativeTransform,
                                    TEXT("itemHand_l"));

            mesh->RelativeLocation = takeComp->_locationAttach;
            mesh->RelativeRotation = takeComp->_rotationAttach;
        }
        _itemLeft->SetActorEnableCollision(false);
        _activeScenaryItems.Remove(data.actor);
    }
}

void APlayerCharacter::DropItemLeft() {
    UStaticMeshComponent* mesh = _itemLeft->GetStaticMeshComponent();
    if (mesh) {
        mesh->DetachFromParent();
        mesh->SetSimulatePhysics(true);
    }
    _itemLeft->SetActorEnableCollision(true);
    _itemLeft = nullptr;
}

void APlayerCharacter::TakeItemRight() {
    ItemData data = FindItemAndComponents(UItemTakeRight::StaticClass());
    UItemTakeRight* takeComp = data.actor ? Cast<UItemTakeRight>(data.components[0]) : nullptr;

    if (takeComp) {
        _itemRight = data.actor;
        UStaticMeshComponent* mesh = _itemRight->GetStaticMeshComponent();
        if (mesh) {
            mesh->SetSimulatePhysics(false);

            mesh->AttachToComponent(GetMesh(),
                                    FAttachmentTransformRules::KeepRelativeTransform,
                                    TEXT("itemHand_r"));

            mesh->RelativeLocation = takeComp->_locationAttach;
            mesh->RelativeRotation = takeComp->_rotationAttach;
        }
        _itemRight->SetActorEnableCollision(false);
        _activeScenaryItems.Remove(data.actor);
    }
}

void APlayerCharacter::DropItemRight() {
    UStaticMeshComponent* mesh = _itemRight->GetStaticMeshComponent();
    if (mesh) {
        mesh->DetachFromParent();
        mesh->SetSimulatePhysics(true);
    }
    _itemRight->SetActorEnableCollision(true);
    _itemRight = nullptr;
}

/*** SAVING ***/
void APlayerCharacter::SaveInventory(AItemActor* item) {
    UInventory* inventory = Cast<UInventory>(FindComponentByClass(UInventory::StaticClass()));
    if (inventory && item->FindComponentByClass(UItemSave::StaticClass())) {
        inventory->AddItem(item);
    }
}

/****************************************** AUXILIAR FUNCTIONS ***********************************/

/*** OUTSIDE ***/
void APlayerCharacter::ActivateScenaryItem(AItemActor* item) {
    _activeScenaryItems.AddUnique(item);
}

void APlayerCharacter::DeactivateScenaryItem(AItemActor* item) {
    _activeScenaryItems.Remove(item);
}

ItemData APlayerCharacter::FindItemAndComponents(const TSubclassOf<UActorComponent> ComponentClass) {
    ItemData res{};
    int i = _activeScenaryItems.Num() - 1;
    while(!res.actor && i >= 0) {
        res.components = _activeScenaryItems[i]->GetComponentsByClass(ComponentClass);
        if (res.components.Num() > 0) {
            res.actor = _activeScenaryItems[i];
        }
        else {
            i--;
        }
    }
    return res;
}