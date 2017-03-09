// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "PlayerControllerPlay.h"

#include "GameModePlay.h"
#include "NWGameInstance.h"


APlayerControllerPlay::APlayerControllerPlay(const FObjectInitializer& OI) : Super(OI) {}

void APlayerControllerPlay::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("Menu", IE_Released, this, &APlayerControllerPlay::ToogleMenu);
    InputComponent->BindAction("Exit", IE_Released, this, &APlayerControllerPlay::ExitGame);
}

void APlayerControllerPlay::BeginPlay() {
    Super::BeginPlay();

    if (IsLocalController()) {
        UNWGameInstance* gameInstance = Cast<UNWGameInstance>(GetGameInstance());
        if (gameInstance) SERVER_CallUpdate(gameInstance->_PlayerInfoSaved);
    }
}

bool APlayerControllerPlay::SERVER_CallUpdate_Validate(FPlayerInfo info) {
    return true; 
}
void APlayerControllerPlay::SERVER_CallUpdate_Implementation(FPlayerInfo info) {
    AGameModePlay* gameMode = Cast<AGameModePlay>(GetWorld()->GetAuthGameMode());
    if (gameMode) gameMode->SERVER_RespawnPlayer(this, info);
}

/****************************************** ACTION MAPPINGS **************************************/
/*************** TRIGGER MENU *************/
void APlayerControllerPlay::ToogleMenu() {
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

void APlayerControllerPlay::ExitGame() {
    FGenericPlatformMisc::RequestExit(false);
}