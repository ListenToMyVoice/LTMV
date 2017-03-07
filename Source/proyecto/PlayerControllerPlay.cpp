// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "PlayerControllerPlay.h"

#include "GameModePlay.h"
#include "NWGameInstance.h"


void APlayerControllerPlay::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APlayerControllerPlay, _PlayerInfo);
}

APlayerControllerPlay::APlayerControllerPlay(const FObjectInitializer& OI) : Super(OI) {}

void APlayerControllerPlay::BeginPlay() {
    Super::BeginPlay();

    if (IsLocalController()) {
        UNWGameInstance* gameInstance = Cast<UNWGameInstance>(GetGameInstance());
        if (gameInstance) {
            _PlayerInfo = gameInstance->_PlayerInfoSaved;// In client...
        }
        SERVER_PassInfoToServer(_PlayerInfo);
    }
}

void APlayerControllerPlay::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("Menu", IE_Released, this, &APlayerControllerPlay::ToogleMenu);
}

bool APlayerControllerPlay::SERVER_PassInfoToServer_Validate(FPlayerInfo info) { return true; }
void APlayerControllerPlay::SERVER_PassInfoToServer_Implementation(FPlayerInfo info) {
    _PlayerInfo = info;//... and Server

    AGameModePlay* gameMode = Cast<AGameModePlay>(GetWorld()->GetAuthGameMode());
    if (gameMode) {
        gameMode->SERVER_RespawnPlayer(this, _PlayerInfo.CharacterClass);
    }
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