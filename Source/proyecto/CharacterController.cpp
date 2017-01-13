// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "CharacterController.h"
#include "PlayerCharacter.h"

ACharacterController::ACharacterController() {
    //bReplicates = true;
}

void ACharacterController::BeginPlay() {
    Super::BeginPlay();
}

void ACharacterController::SetupInputComponent() {
    Super::SetupInputComponent();

    InputComponent->BindAction("TakeLeft", IE_Released, this, &ACharacterController::SERVER_TakeLeft);
}

bool ACharacterController::SERVER_TakeLeft_Validate() {
    return true;
}

void ACharacterController::SERVER_TakeLeft_Implementation() {
    APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
    if (HasAuthority()) {
        ULibraryUtils::Log(TEXT("SERVER SERVER_TakeLeft_Implementation"), 3);
        //player->TakeLeft();
        TakeLeft();
    }
    //else {
    //    ULibraryUtils::Log(TEXT("CLIENT"), 3);
    //    player->TakeLeft();
    //}
}

void ACharacterController::TakeLeft_Implementation() {
    ULibraryUtils::Log(TEXT("CLIENT TakeLeft_Implementation"), 3);
    APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
    player->TakeLeft();
}