// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "VRPlayerController.h"

/* VR Includes */
#include "HeadMountedDisplay.h"

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
    IHeadMountedDisplay* HMD = (IHeadMountedDisplay*)(GEngine->HMDDevice.Get());

    FQuat quaternion;
    FVector position;
    HMD->GetCurrentOrientationAndPosition(quaternion, position);
    FRotator rotation = quaternion.Rotator();

    _Menu = GetWorld()->SpawnActor(_MenuClass, &position, &rotation);
}

void AVRPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();

    InputComponent->BindAction("Menu", IE_Released, this, &AVRPlayerController::ToogleMenu);
}

/****************************************** ACTION MAPPINGS **************************************/
/*************** TRIGGER MENU *************/
void AVRPlayerController::ToogleMenu() {
    bool enable = _Menu->IsActorTickEnabled();
    _Menu->SetActorHiddenInGame(enable);
    _Menu->SetActorTickEnabled(!enable);
}