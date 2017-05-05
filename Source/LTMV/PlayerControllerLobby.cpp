// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "PlayerControllerLobby.h"

#include "GameModeLobby.h"
#include "PlayerCharacter.h"
#include "MenuLobby.h"


APlayerControllerLobby::APlayerControllerLobby(const FObjectInitializer& OI) : Super(OI) {
    GConfig->GetString(
        TEXT("/Script/EngineSettings.GameMapsSettings"),
        TEXT("GameDefaultMap"),
        _MapMainMenu,
        GEngineIni
    );

    /* MENU INTERFACE */
    _MenuClass = AMenuLobby::StaticClass();
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

void APlayerControllerLobby::CLIENT_CreateMenu_Implementation(TSubclassOf<AActor> MenuClass) {
    /* MENU INTERFACE */
    if (GetPawnOrSpectator()) {
        _MenuActor = Cast<AMenu>(GetWorld()->SpawnActor(MenuClass));

        UCameraComponent* CameraComp = Cast<UCameraComponent>(GetPawnOrSpectator()->
                                                              FindComponentByClass<UCameraComponent>());
        if (CameraComp) {
            _MenuActor->ToogleMenu(CameraComp->GetComponentLocation(),
                                   CameraComp->GetComponentRotation());

            APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
            if (PlayerCharacter) PlayerCharacter->ToggleMenuInteraction(!_MenuActor->_IsMenuHidden);
        }
    }
}

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

void APlayerControllerLobby::OnFindSessionsComplete(FString SessionOwner) {
    if (_MenuActor) _MenuActor->OnFindSessionComplete(SessionOwner);
}

/****************************************** ACTION MAPPINGS **************************************/
/******** USE ITEM LEFT *********/
void APlayerControllerLobby::UseLeftPressed() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) {
        bool  IsMenuHidden = _MapMainMenu.Contains(GetWorld()->GetMapName()) ? false : 
                                _MenuActor ? _MenuActor->_IsMenuHidden : true;
        PlayerCharacter->UseLeftPressed(IsMenuHidden);
    }
}

void APlayerControllerLobby::UseLeftReleased() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) {
        bool  IsMenuHidden = _MapMainMenu.Contains(GetWorld()->GetMapName()) ? false :
                                _MenuActor ? _MenuActor->_IsMenuHidden : true;
        PlayerCharacter->UseLeftReleased(IsMenuHidden);
    }
}

/*************** TRIGGER MENU *************/
void APlayerControllerLobby::ToogleMenu() {
    if (!_MapMainMenu.Contains(GetWorld()->GetMapName())) {
        /* MENU INTERFACE */
        if (!_MenuActor) _MenuActor = Cast<AMenu>(GetWorld()->SpawnActor(_MenuClass));

        if (GetPawnOrSpectator()) {
            UCameraComponent* CameraComp = Cast<UCameraComponent>(GetPawnOrSpectator()->
                                                                  FindComponentByClass<UCameraComponent>());
            if (CameraComp) {
                _MenuActor->ToogleMenu(CameraComp->GetComponentLocation(),
                                       CameraComp->GetComponentRotation());

                APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
                if (PlayerCharacter) PlayerCharacter->ToggleMenuInteraction(!_MenuActor->_IsMenuHidden);
            }
        }
    }
}