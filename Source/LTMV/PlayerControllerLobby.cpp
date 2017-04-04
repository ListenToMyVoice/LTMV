// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "PlayerControllerLobby.h"

#include "GameModeLobby.h"


APlayerControllerLobby::APlayerControllerLobby(const FObjectInitializer& OI) : Super(OI) {}

void APlayerControllerLobby::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("Exit", IE_Released, this, &APlayerControllerLobby::ExitGame);
}

void APlayerControllerLobby::Client_InitialSetup_Implementation() {
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

void APlayerControllerLobby::Client_CreateMenu_Implementation(TSubclassOf<AActor> menuClass) {
    APawn* pawn = GetPawn();
    if (pawn) {
        UCameraComponent* cameraComp = Cast<UCameraComponent>(pawn->FindComponentByClass<UCameraComponent>());
        if (cameraComp) {
            if (_ActorWidgetMenu) _ActorWidgetMenu->Destroy();
            FVector position = cameraComp->GetComponentLocation();
            FRotator rotation = cameraComp->GetComponentRotation();

            _ActorWidgetMenu = GetWorld()->SpawnActor(menuClass, &position, &rotation);
        }
    }
}

/****************************************** ACTION MAPPINGS **************************************/
/***************** EXIT GAME **************/
void APlayerControllerLobby::ExitGame() {
    FGenericPlatformMisc::RequestExit(false);
}