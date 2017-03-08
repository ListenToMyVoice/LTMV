// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "PlayerControllerLobby.h"

#include "GameModeLobby.h"
#include "NWGameInstance.h"

void APlayerControllerLobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APlayerControllerLobby, _PlayerInfo);
    DOREPLIFETIME(APlayerControllerLobby, _OtherPlayerInfo);
}

APlayerControllerLobby::APlayerControllerLobby(const FObjectInitializer& OI) : Super(OI) {}

void APlayerControllerLobby::BeginPlay() {
    Super::BeginPlay();
}

void APlayerControllerLobby::Client_InitialSetup_Implementation() {
    UNWGameInstance* gameInstance = Cast<UNWGameInstance>(GetGameInstance());
    if (gameInstance) {
        _PlayerInfo = gameInstance->_PlayerInfoSaved;// In client...
    }
    SERVER_CallUpdate(_PlayerInfo, false);
}

bool APlayerControllerLobby::SERVER_CallUpdate_Validate(FPlayerInfo info, bool changedStatus) { return true; }
void APlayerControllerLobby::SERVER_CallUpdate_Implementation(FPlayerInfo info, bool changedStatus) {
    _PlayerInfo = info;//... and Server

    AGameModeLobby* gameMode = Cast<AGameModeLobby>(GetWorld()->GetAuthGameMode());
    if (gameMode) {
        gameMode->SERVER_SwapCharacter(this, _PlayerInfo.CharacterClass, changedStatus);
        gameMode->SERVER_UpdateEveryOne();
    }
}

void APlayerControllerLobby::Client_UpdateLobbySettings_Implementation(const FString& mapName) {
    //TO DO
}

void APlayerControllerLobby::Client_UpdateNumberOfPlayers_Implementation(int32 CurrentPlayers,
                                                                         int32 MaxPlayers) {
    //TO DO
}

void APlayerControllerLobby::Client_AddPlayerInfo_Implementation(FPlayerInfo playerInfo) {
    _OtherPlayerInfo = playerInfo;
    //TO DO
}

void APlayerControllerLobby::Client_CreateMenu_Implementation(TSubclassOf<AActor> menuClass) {
    APawn* pawn = GetPawn();
    if (pawn) {
        UCameraComponent* cameraComp = Cast<UCameraComponent>(pawn->FindComponentByClass<UCameraComponent>());
        if (cameraComp) {
            if (_ActorWidgetMenu) _ActorWidgetMenu->Destroy();
            //FVector position = cameraComp->GetForwardVector() * 1000.0f +
            //    cameraComp->GetComponentLocation();
            FVector position = cameraComp->GetComponentLocation();
            FRotator rotation = cameraComp->GetComponentRotation();

            _ActorWidgetMenu = GetWorld()->SpawnActor(menuClass, &position, &rotation);
        }
    }
}