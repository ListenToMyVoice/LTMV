// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter() {
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    //GetCharacterMovement()->bOrientRotationToMovement = true;
    //GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); /* (Y,Z,X) */

    // Create a CameraComponent	
    //FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
    //FirstPersonCameraComponent->SetupAttachment(GetMesh(), TEXT("FPVCamera"));

    //FirstPersonCameraComponent->AttachTo(GetMesh(), TEXT("FPVCamera"), EAttachLocation::KeepRelativeOffset, true);

    //FirstPersonCameraComponent->RelativeLocation = FVector(0.f, 0.f, 0.f);
    //FirstPersonCameraComponent->RelativeRotation = FRotator(0.f, 0.f, 0.f);
    //FirstPersonCameraComponent->bUsePawnControlRotation = true;

    // Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
    //Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
    //Mesh1P->SetOnlyOwnerSee(true);
    //Mesh1P->SetupAttachment(FirstPersonCameraComponent);
    //Mesh1P->bCastDynamicShadow = false;
    //Mesh1P->CastShadow = false;
    //Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
    //Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
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

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

    //Super::SetupPlayerInputComponent(InputComponent);
    //InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    //InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    //InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
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

    //itemActor->GetStaticMeshComponent()->RelativeLocation = FVector(0.f, 0.f, 0.f);
    //itemActor->GetStaticMeshComponent()->RelativeRotation = FRotator(0.f, 0.f, 0.f);
    
    //itemActor->GetStaticMeshComponent()->RelativeLocation =
    //    GetMesh()->GetSocketLocation("itemHand_r");
    //itemActor->GetStaticMeshComponent()->RelativeRotation =
    //    GetMesh()->GetSocketRotation("itemHand_r");
}