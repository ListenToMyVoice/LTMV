// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "VRPlayerController.h"

#include "VRCharacter.h"

AVRPlayerController::AVRPlayerController(const FObjectInitializer& OI) : Super(OI) {
    static ConstructorHelpers::FClassFinder<AActor> ClassFinder(TEXT("/Game/Maps/Alex/MenuActor"));
    _MenuClass = ClassFinder.Class;
    _Menu = nullptr;
}

void AVRPlayerController::BeginPlay() {
    Super::BeginPlay();

    /* Create Menu */
    CreateMenu();
}


void AVRPlayerController::CreateMenu() {
    FVector position(0.0f, 0.0f, 200.0f);
    FRotator rotation(0.0f, 0.0f, 0.0f);
    _Menu = GetWorld()->SpawnActor(_MenuClass, &position, &rotation);
    isMenuHidden = false;
}

void AVRPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();
    UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent"));
    InputComponent->BindAction("Menu", IE_Released, this, &AVRPlayerController::ToogleMenu);
}

/****************************************** ACTION MAPPINGS **************************************/
/*************** TRIGGER MENU *************/
void AVRPlayerController::ToogleMenu() {
    if (isMenuHidden) {
        ULibraryUtils::SetActorEnable(_Menu);

        AVRCharacter* character = Cast<AVRCharacter>(GetPawn());
        if (character) {
            FVector position = character->CameraComp->GetForwardVector() * 200.0f +
                               character->CameraComp->GetComponentLocation();
            FRotator rotation = character->CameraComp->GetComponentRotation();
            _Menu->SetActorLocationAndRotation(position,
                                               rotation,
                                               false,
                                               nullptr,
                                               ETeleportType::TeleportPhysics);
        }
    }
    else {
        ULibraryUtils::SetActorEnable(_Menu, false);
    }
    isMenuHidden = !isMenuHidden;
}