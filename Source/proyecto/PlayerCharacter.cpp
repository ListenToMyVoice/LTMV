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

    _activeScenaryItem = nullptr;
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
    if (_itemLeft && _activeScenaryItem) {
        // REPLACE
        DropItemLeft();
        TakeItemLeft();
    }
    else if (_itemLeft && !_activeScenaryItem) {
        // DROP
        DropItemLeft();
    }
    else if (!_itemLeft && _activeScenaryItem) {
        // TAKE
        TakeItemLeft();
    }
}

void APlayerCharacter::TakeRight() {
    if (_itemRight && _activeScenaryItem) {
        // REPLACE
        DropItemRight();
        TakeItemRight();
    }
    else if (_itemRight && !_activeScenaryItem) {
        // DROP
        DropItemRight();
    }
    else if (!_itemRight && _activeScenaryItem) {
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

/****************************************** AUXILIAR FUNCTIONS ***********************************/

/*** TAKING DROPING ***/
void APlayerCharacter::TakeItemLeft() {
    UActorComponent* comp = _activeScenaryItem->FindComponentByClass(UItemTakeLeft::StaticClass());
    UItemTakeLeft* takeComp = Cast<UItemTakeLeft>(comp);

    if (takeComp) {
        _itemLeft = _activeScenaryItem;
        UStaticMeshComponent* mesh = _activeScenaryItem->GetStaticMeshComponent();
        if (mesh) {
            mesh->SetSimulatePhysics(false);

            mesh->AttachToComponent(GetMesh(),
                                    FAttachmentTransformRules::KeepRelativeTransform,
                                    TEXT("itemHand_l"));

            mesh->RelativeLocation = takeComp->_locationAttach;
            mesh->RelativeRotation = takeComp->_rotationAttach;
        }
        _itemLeft->SetActorEnableCollision(false);
        _activeScenaryItem = nullptr;
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
    UActorComponent* comp = _activeScenaryItem->FindComponentByClass(UItemTakeRight::StaticClass());
    UItemTakeRight* takeComp = Cast<UItemTakeRight>(comp);

    if (takeComp) {
        _itemRight = _activeScenaryItem;
        UStaticMeshComponent* mesh = _activeScenaryItem->GetStaticMeshComponent();
        if (mesh) {
            mesh->SetSimulatePhysics(false);

            mesh->AttachToComponent(GetMesh(),
                                    FAttachmentTransformRules::KeepRelativeTransform,
                                    TEXT("itemHand_r"));

            mesh->RelativeLocation = takeComp->_locationAttach;
            mesh->RelativeRotation = takeComp->_rotationAttach;
        }
        _itemRight->SetActorEnableCollision(false);
        _activeScenaryItem = nullptr;
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
    //UActorComponent* comp = item->FindComponentByClass(UItemSave::StaticClass());
    //UItemSave* takeComp = Cast<UItemSave>(comp);

    UInventory* inventory = Cast<UInventory>(FindComponentByClass(UInventory::StaticClass()));
    if (inventory && item->FindComponentByClass(UItemSave::StaticClass())) {
        inventory->AddItem(item);
    }
}

/*** OUTSIDE ***/
void APlayerCharacter::ActivateScenaryItem(AItemActor* item) {
    if (_activeScenaryItem != item) {
        _activeScenaryItem = item;
        ULibraryUtils::Log(item->_name.ToString());
    }
}

void APlayerCharacter::DeactivateScenaryItem(AItemActor* item) {
    if (_activeScenaryItem == item) {
        _activeScenaryItem = nullptr;
        ULibraryUtils::Log(item->_name.ToString(), 2);
    }
}

/* OUTSIDE ACTION MAPPINGS */
//bool APlayerCharacter::TakeLeft(AStaticMeshActor* itemActor, FVector &location, FRotator &rotation) {
//    if (!_busyLeft && itemActor != nullptr) {
//        itemActor->GetStaticMeshComponent()->SetSimulatePhysics(false);
//        itemActor->SetActorEnableCollision(false);
//        //itemActor->SetActorTickEnabled(false);
//        
//        itemActor->GetStaticMeshComponent()->AttachToComponent(GetMesh(),
//            FAttachmentTransformRules::KeepRelativeTransform, TEXT("itemHand_l"));
//
//        itemActor->GetStaticMeshComponent()->RelativeLocation = location;
//        itemActor->GetStaticMeshComponent()->RelativeRotation = rotation;
//
//        _busyLeft = true;
//        ULibraryUtils::Log(TEXT("TakeLeft"), 3);
//        return true;
//    }
//    return false;
//}
//
//bool APlayerCharacter::DropLeft(AStaticMeshActor* itemActor) {
//    if (_busyLeft && itemActor != nullptr) {
//        itemActor->GetStaticMeshComponent()->DetachFromParent();
//        itemActor->SetActorEnableCollision(true);
//        itemActor->GetStaticMeshComponent()->SetSimulatePhysics(true);
//
//        _busyLeft = false;
//        ULibraryUtils::Log(TEXT("DropLeft"), 2);
//        return true;
//    }
//    return false;
//}
//
//bool APlayerCharacter::TakeRight(AStaticMeshActor* itemActor, FVector &location, FRotator &rotation) {
//    if (!_busyRight && itemActor != nullptr) {
//        itemActor->GetStaticMeshComponent()->SetSimulatePhysics(false);
//        itemActor->SetActorEnableCollision(false);
//
//        itemActor->GetStaticMeshComponent()->AttachToComponent(GetMesh(),
//            FAttachmentTransformRules::KeepRelativeTransform, TEXT("itemHand_r"));
//
//        itemActor->GetStaticMeshComponent()->RelativeLocation = location;
//        itemActor->GetStaticMeshComponent()->RelativeRotation = rotation;
//        
//        _busyRight = true;
//        ULibraryUtils::Log(TEXT("TakeRight"), 3);
//        return true;
//    }
//    return false;
//}
//
//bool APlayerCharacter::DropRight(AStaticMeshActor* itemActor) {
//    if (_busyRight && itemActor != nullptr) {
//        itemActor->GetStaticMeshComponent()->DetachFromParent();
//        itemActor->SetActorEnableCollision(true);
//        itemActor->GetStaticMeshComponent()->SetSimulatePhysics(true);
//
//        _busyRight = false;
//        ULibraryUtils::Log(TEXT("DropRight"), 2);
//        return true;
//    }
//    return false;
//}