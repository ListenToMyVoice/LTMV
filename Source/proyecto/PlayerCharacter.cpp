// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter() {
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // set our turn rates for input
    _baseTurnRate = 45.f;
    _baseLookUpRate = 45.f;

    _busyLeft = false;
    _busyRight = false;
}

void APlayerCharacter::BeginPlay() {
    Super::BeginPlay();
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
    check(PlayerInputComponent);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
}

/* ACTION MAPPINGS */
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

/* OUTSIDE ACTION MAPPINGS */
void APlayerCharacter::TakeLeft(AStaticMeshActor* itemActor, FVector &location, FRotator &rotation) {
    if (itemActor != nullptr) {
        itemActor->SetActorEnableCollision(false);
        itemActor->GetStaticMeshComponent()->AttachToComponent(GetMesh(),
            FAttachmentTransformRules::KeepRelativeTransform, TEXT("itemHand_l"));

        itemActor->GetStaticMeshComponent()->RelativeLocation = location;
        itemActor->GetStaticMeshComponent()->RelativeRotation = rotation;
        ULibraryUtils::Log(TEXT("TakeLeft"), 3);
    }
}

void APlayerCharacter::TakeRight(AStaticMeshActor* itemActor, FVector &location, FRotator &rotation) {
    if (!_busyRight && itemActor != nullptr) {
        itemActor->SetActorEnableCollision(false);
        itemActor->GetStaticMeshComponent()->AttachToComponent(GetMesh(),
            FAttachmentTransformRules::KeepRelativeTransform, TEXT("itemHand_r"));

        itemActor->GetStaticMeshComponent()->RelativeLocation = location;
        itemActor->GetStaticMeshComponent()->RelativeRotation = rotation;
        
        _busyRight = true;
        ULibraryUtils::Log(TEXT("TakeRight"), 3);
    }
}

void APlayerCharacter::DropRight(AStaticMeshActor* itemActor) {
    if (_busyRight && itemActor != nullptr) {
        itemActor->GetStaticMeshComponent()->DetachFromParent();
        itemActor->SetActorEnableCollision(true);

        _busyRight = false;
        ULibraryUtils::Log(TEXT("DropRight"), 2);
    }
}

void APlayerCharacter::SaveItem(AStaticMeshActor* itemActor) {
    AItem* item = Cast<AItem>(itemActor);
    if (item != nullptr) {
        UInventory* inventory = Cast<UInventory>(FindComponentByClass(UInventory::StaticClass()));
        if (inventory != nullptr) {
            inventory->AddItem(item);
        }
    }
}