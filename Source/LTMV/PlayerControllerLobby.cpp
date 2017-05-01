// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "PlayerControllerLobby.h"

#include "GameModeLobby.h"
#include "PlayerCharacter.h"
#include "NWGameInstance.h"


APlayerControllerLobby::APlayerControllerLobby(const FObjectInitializer& OI) : Super(OI) {
    GConfig->GetString(
        TEXT("/Script/EngineSettings.GameMapsSettings"),
        TEXT("GameDefaultMap"),
        _MapMainMenu,
        GEngineIni
    );
}

void APlayerControllerLobby::BeginPlay() {
    Super::BeginPlay();

    if (IsLocalController()) {
        _GameInstance = Cast<UNWGameInstance>(GetGameInstance());
    }
}

void APlayerControllerLobby::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("Menu", IE_Released, this, &APlayerControllerLobby::ToogleMenu);

    /* USE ITEM */
    InputComponent->BindAction("ClickLeft", IE_Pressed, this, &APlayerControllerLobby::UseLeftPressed);
    InputComponent->BindAction("ClickLeft", IE_Released, this, &APlayerControllerLobby::UseLeftReleased);
}

void APlayerControllerLobby::CLIENT_InitialSetup_Implementation() {
    UNWGameInstance* gameInstance = Cast<UNWGameInstance>(GetGameInstance());
    if (gameInstance) SERVER_CallUpdate(gameInstance->_PlayerInfoSaved, false);
}

bool APlayerControllerLobby::SERVER_CallUpdate_Validate(FPlayerInfo info,
                                                        bool changedStatus) { return true; }
void APlayerControllerLobby::SERVER_CallUpdate_Implementation(FPlayerInfo info,
                                                              bool changedStatus) {
    AGameModeLobby* gameMode = Cast<AGameModeLobby>(GetWorld()->GetAuthGameMode());
    if (gameMode) gameMode->SERVER_SwapCharacter(this, info, changedStatus);
}

//void APlayerControllerLobby::CLIENT_CreateMenu_Implementation(TSubclassOf<AActor> menuClass) {
//    APawn* pawn = GetPawn();
//    if (pawn) {
//        UCameraComponent* cameraComp = Cast<UCameraComponent>(pawn->FindComponentByClass<UCameraComponent>());
//        if (cameraComp) {
//            FVector position = cameraComp->GetComponentLocation();
//            FRotator rotation = cameraComp->GetComponentRotation();
//
//            // Save a Reference of the spawned actor??
//            GetWorld()->SpawnActor(menuClass, &position, &rotation);
//        }
//    }
//}

void APlayerControllerLobby::AfterPossessed() {
    /* CLIENT-SERVER EXCEPTION  */
    if (!_ClientPossesed) {
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
        if (PlayerCharacter) {
            PlayerCharacter->AfterPossessed(false);
            _ClientPossesed = true;
        }
    }
}

void APlayerControllerLobby::OnRep_Pawn() {
    Super::OnRep_Pawn();
    /* CLIENT-SERVER EXCEPTION  */
    AfterPossessed();
}

/****************************************** ACTION MAPPINGS **************************************/
/******** USE ITEM LEFT *********/
void APlayerControllerLobby::UseLeftPressed() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) {
        bool  MenuBool = _MapMainMenu.Contains(GetWorld()->GetMapName()) ? false : _GameInstance->_IsMenuHidden;
        PlayerCharacter->UseLeftPressed(MenuBool);
    }
}

void APlayerControllerLobby::UseLeftReleased() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) {
        bool  MenuBool = _MapMainMenu.Contains(GetWorld()->GetMapName()) ? false : _GameInstance->_IsMenuHidden;
        PlayerCharacter->UseLeftReleased(MenuBool);
    }
}

/*************** TRIGGER MENU *************/
void APlayerControllerLobby::ToogleMenu() {
    if (GetPawnOrSpectator() && _GameInstance) {
        UCameraComponent* CameraComp = Cast<UCameraComponent>(GetPawnOrSpectator()->
                                                              FindComponentByClass<UCameraComponent>());

        if (CameraComp) {
            _GameInstance->ToogleMenu(CameraComp->GetComponentLocation(),
                                      CameraComp->GetComponentRotation());

            APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
            if (PlayerCharacter) PlayerCharacter->ToggleMenuInteraction(!_GameInstance->_IsMenuHidden);
        }
    }
}