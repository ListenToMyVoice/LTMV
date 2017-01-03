// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter() {
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;
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
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate) {
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

/* OUTSIDE ACTION MAPPINGS */
void APlayerCharacter::TakeItem(AStaticMeshActor* itemActor, FVector &location, FRotator &rotation) {
    UActorComponent * component = itemActor->GetComponentByClass(UBoxComponent::StaticClass());
    if (component) {
        ULibraryUtils::Log(TEXT("BOX DESTROYED"), 3);
        component->DestroyComponent(true);
    }
    else {
        ULibraryUtils::Log(TEXT("BOX NOT DESTROYED"), 3);
    }

    itemActor->GetStaticMeshComponent()->AttachTo(GetMesh(), TEXT("itemHand_r"),
                                                  EAttachLocation::KeepRelativeOffset, true);
    
    itemActor->GetStaticMeshComponent()->RelativeLocation = location;
    itemActor->GetStaticMeshComponent()->RelativeRotation = rotation;
}

void APlayerCharacter::SaveItem(AStaticMeshActor* itemActor) {
    
}