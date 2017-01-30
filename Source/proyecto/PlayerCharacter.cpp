// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "PlayerCharacter.h"

#include "ItemTakeLeft.h"
#include "ItemTakeRight.h"
#include "ItemSave.h"
#include "Inventory.h"
#include "ItfUsable.h"
#include "ItemActor.h"
#include "GameStateLTMV.h"

APlayerCharacter::APlayerCharacter() {
    bReplicates = true;
    bReplicateMovement = true;

    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // set our turn rates for input
    _baseTurnRate = 45.f;
    _baseLookUpRate = 45.f;

    _itemLeft = nullptr;
    _itemRight = nullptr;
    _isAction = false;

    _activeScenaryItems = {};

    RayParameter = 300.f;

    _audioComp =  CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
}

void APlayerCharacter::BeginPlay() {
    Super::BeginPlay();
    GetOwnComponents();
}

void APlayerCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    bool stop = false;
    if (GetCharacterMovement()->Velocity.Equals(FVector::ZeroVector)) {
        stop = true;
    }
    SwitchSound(_walkSound, stop);
}

void APlayerCharacter::GetOwnComponents() {
    TArray<UActorComponent*> Components;
    this->GetComponents(Components);

    for (UActorComponent* Component : Components) {
        if (Component && Component->IsA<UCameraComponent>()) {
            _playerCamera = Cast<UCameraComponent>(Component);
        }
    }
}

FHitResult APlayerCharacter::Raycasting() {

    bool bHitRayCastFlag;
    FHitResult HitActor;
    FCollisionQueryParams CollisionInfo;

    FVector StartRaycast = _playerCamera->GetComponentLocation();
    FVector EndRaycast = _playerCamera->GetForwardVector() * RayParameter + StartRaycast;

    bHitRayCastFlag = GetWorld()->LineTraceSingleByChannel(HitActor, StartRaycast, EndRaycast, ECC_Visibility, CollisionInfo);
    DrawDebugLine(GetWorld(), StartRaycast, EndRaycast, FColor(255, 0, 0), false, -1.0f, (uint8)'\000', 0.8f);

    //if (bHitRayCastFlag && HitActor.Actor.IsValid()) {
    //    // COOL STUFF TO GRAB OBJECTS
    //    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitActor.Actor->GetName()));
    //}

    return HitActor;
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* playerInput) {
    check(playerInput);

    /* MOVEMENT */
    playerInput->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    playerInput->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    playerInput->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    playerInput->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    playerInput->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    playerInput->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
    playerInput->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    playerInput->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

    /* SERVER */
    playerInput->BindAction("TakeLeft", IE_Released, this, &APlayerCharacter::SERVER_TakeLeft);
    playerInput->BindAction("TakeRight", IE_Released, this, &APlayerCharacter::SERVER_TakeRight);
    playerInput->BindAction("SaveLeft", IE_Released, this, &APlayerCharacter::SERVER_SaveLeft);
    playerInput->BindAction("SaveRight", IE_Released, this, &APlayerCharacter::SERVER_SaveRight);
    playerInput->BindAction("Help", IE_Released, this, &APlayerCharacter::SERVER_Help);
    playerInput->BindAction("Use", IE_Released, this, &APlayerCharacter::SERVER_Use);
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

/************ SERVER ************/
bool APlayerCharacter::SERVER_TakeLeft_Validate() { return true; }
bool APlayerCharacter::SERVER_TakeRight_Validate() { return true; }
bool APlayerCharacter::SERVER_SaveLeft_Validate() { return true; }
bool APlayerCharacter::SERVER_SaveRight_Validate() { return true; }
bool APlayerCharacter::SERVER_Help_Validate() { return true; }
bool APlayerCharacter::SERVER_Use_Validate() { return true; }

void APlayerCharacter::SERVER_TakeLeft_Implementation() {
    OnRep_TakeLeft();
}

void APlayerCharacter::SERVER_TakeRight_Implementation() {
    OnRep_TakeRight();
}

void APlayerCharacter::SERVER_SaveLeft_Implementation() {
    OnRep_SaveLeft();
}

void APlayerCharacter::SERVER_SaveRight_Implementation() {
    OnRep_SaveRight();
}

void APlayerCharacter::SERVER_Help_Implementation() {
    OnRep_Help();
}

void APlayerCharacter::SERVER_Use_Implementation() {
    OnRep_Use();
}

/************ CLIENT ************/
void APlayerCharacter::OnRep_TakeLeft_Implementation() {
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

void APlayerCharacter::OnRep_TakeRight_Implementation() {
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

void APlayerCharacter::OnRep_SaveLeft_Implementation() {
    if (_itemLeft) {
        // SAVE
        SaveInventory(_itemLeft);
        _itemLeft = nullptr;
    }
}

void APlayerCharacter::OnRep_SaveRight_Implementation() {
    if (_itemRight) {
        // SAVE
        SaveInventory(_itemRight);
        _itemRight = nullptr;
    }
}

void APlayerCharacter::OnRep_Help_Implementation() {
    ULibraryUtils::Log(TEXT("ACTIVE ITEMS:"), 3);
    for (AItemActor* itemActor : _activeScenaryItems) {
        ULibraryUtils::Log(itemActor->_name.ToString(), 3);
    }
}

void APlayerCharacter::OnRep_Use_Implementation() {
    bool found = false;
    int i = _activeScenaryItems.Num() - 1;
    while (!found && i >= 0) {
        const TSet <UActorComponent*> set = _activeScenaryItems[i]->GetComponents();

        for (UActorComponent* component : set) {
            if (component->GetClass()->ImplementsInterface(UItfUsable::StaticClass())) {
                IItfUsable* itfObject = Cast<IItfUsable>(component);
                if (itfObject) itfObject->Execute_Use(component);
            }
        }
        i--;
    }
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
        mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
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
        //FDetachmentTransformRules rules(EDetachmentRule::KeepWorld, true);
        //mesh->DetachFromComponent(rules);
        mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
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
    _isAction = true;
}

void APlayerCharacter::DeactivateScenaryItem(AItemActor* item) {
    _activeScenaryItems.Remove(item);
    _isAction = _activeScenaryItems.Num() > 0 ? true : false;
}

ItemData APlayerCharacter::FindItemAndComponents(const TSubclassOf<UActorComponent> ComponentClass) {
    ItemData res{};
    int i = _activeScenaryItems.Num() - 1;
    while (!res.actor && i >= 0) {
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

void APlayerCharacter::SwitchSound(USoundWave* sound, bool stop) {
    if (_audioComp) {
        if (stop && _audioComp->IsPlaying()) {
            _audioComp->Stop();
        }
        else {
            if (!_audioComp->IsPlaying()) {
                _audioComp->SetSound(sound);
                _audioComp->Play();
            }
        }
    }
}