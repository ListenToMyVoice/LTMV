// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "VRPlayerController.h"


AVRPlayerController::AVRPlayerController(const FObjectInitializer& OI) : Super(OI) {}

void AVRPlayerController::BeginPlay() {
    Super::BeginPlay();
}

void AVRPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("Menu", IE_Released, this, &AVRPlayerController::ToogleMenu);
}

/****************************************** ACTION MAPPINGS **************************************/
/*************** TRIGGER MENU *************/
void AVRPlayerController::ToogleMenu() {
    //if (isMenuHidden) {
    //    ULibraryUtils::SetActorEnable(_Menu);

    //    UCameraComponent* cameraComp = Cast<UCameraComponent>(
    //                                        GetPawn()->FindComponentByClass<UCameraComponent>());
    //    if (cameraComp) {
    //        FVector position = cameraComp->GetForwardVector() * 200.0f +
    //                           cameraComp->GetComponentLocation();
    //        FRotator rotation = cameraComp->GetComponentRotation();
    //        _Menu->SetActorLocationAndRotation(position,
    //                                           rotation,
    //                                           false,
    //                                           nullptr,
    //                                           ETeleportType::TeleportPhysics);
    //    }
    //}
    //else {
    //    ULibraryUtils::SetActorEnable(_Menu, false);
    //}
    //isMenuHidden = !isMenuHidden;
}