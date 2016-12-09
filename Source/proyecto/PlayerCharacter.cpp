// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter() {
    PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); /* (Y,Z,X) */
    GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void APlayerCharacter::BeginPlay() {
    Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
    InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

void APlayerCharacter::MoveForward(float Value) {
    if ((Controller != NULL) && (Value != 0.0f)) {
        const FRotator Rotation = Controller->GetControlRotation();

        const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void APlayerCharacter::MoveRight(float Value) {
    if ((Controller != NULL) && (Value != 0.0f)) {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}