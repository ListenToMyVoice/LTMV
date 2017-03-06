// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "GameModeLobby.h"

#include "NWGameInstance.h"

void AGameModeLobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AGameModeLobby, _ServerName);
    DOREPLIFETIME(AGameModeLobby, _MapNameGM);
    DOREPLIFETIME(AGameModeLobby, _MaxPlayers);
    DOREPLIFETIME(AGameModeLobby, _CurrentPlayers);

    DOREPLIFETIME(AGameModeLobby, _PlayerController_1);
    DOREPLIFETIME(AGameModeLobby, _PlayerController_2);
    DOREPLIFETIME(AGameModeLobby, _SpawnPoint_1);
    DOREPLIFETIME(AGameModeLobby, _SpawnPoint_2);
}

AGameModeLobby::AGameModeLobby(const class FObjectInitializer& OI) : Super(OI) {
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT(
        "/Game/BluePrints/PlayerCharacter_BP"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;
    PlayerControllerClass = APlayerControllerLobby::StaticClass();

    bUseSeamlessTravel = true;

    _MaxPlayers = 2;
    _CurrentPlayers = 0;
    _ServerName = "";

    _MapNameGM = "/Game/Maps/Alex/test_map";
}

void AGameModeLobby::BeginPlay() {
    Super::BeginPlay();
}

void AGameModeLobby::PostLogin(APlayerController* NewPlayer) {
    //Super::PostLogin(NewPlayer);
    
    if (HasAuthority()) {
        if (_CurrentPlayers == 0) {//HOST
            _PlayerController_1 = NewPlayer;
        }
        else {//GUEST
            _PlayerController_2 = NewPlayer;
        }
        _CurrentPlayers++;
        
        UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetGameInstance());
        if (GameInstance) {
            _ServerName = GameInstance->_ServerName;
            _MaxPlayers = GameInstance->_MaxPlayers;
        }

        APlayerControllerLobby* PlayerController = Cast<APlayerControllerLobby>(NewPlayer);
        if (PlayerController) {
            PlayerController->Client_InitialSetup();
            PlayerController->Client_UpdateLobbySettings(_MapNameGM);
        }
    }
}

bool AGameModeLobby::SERVER_SwapCharacter_Validate(APlayerController* PlayerController,
                                                   TSubclassOf<ACharacter> CharacterClass,
                                                   bool ChangeStatus) { return true; }
void AGameModeLobby::SERVER_SwapCharacter_Implementation(APlayerController* PlayerController,
                                                         TSubclassOf<ACharacter> CharacterClass,
                                                         bool ChangeStatus) {
    if (!ChangeStatus) {
        if (PlayerController->GetPawn()) {
            PlayerController->GetPawn()->Destroy();
        }
        FindSpawnPoints();
        FTransform transform;
        if (_CurrentPlayers == 1) {
            transform = _SpawnPoint_1->GetActorTransform();
        }
        else {
            transform = _SpawnPoint_2->GetActorTransform();
        }
        APawn* actor = Cast<APawn>(GetWorld()->SpawnActor(CharacterClass, &transform));

        if (actor) PlayerController->Possess(actor);
    }
}

bool AGameModeLobby::SERVER_UpdateEveryOne_Validate() { return true; }
void AGameModeLobby::SERVER_UpdateEveryOne_Implementation() {
    APlayerControllerLobby* playerController1 = Cast<APlayerControllerLobby>(_PlayerController_1);
    APlayerControllerLobby* playerController2 = Cast<APlayerControllerLobby>(_PlayerController_2);
    if (playerController1 && playerController2) {

        playerController1->Client_UpdateNumberOfPlayers(_CurrentPlayers, _MaxPlayers);
        playerController1->Client_AddPlayerInfo(playerController2->_PlayerInfo);

        playerController2->Client_UpdateNumberOfPlayers(_CurrentPlayers, _MaxPlayers);
        playerController2->Client_AddPlayerInfo(playerController1->_PlayerInfo);
    }
}

void AGameModeLobby::LaunchGame() {
    GetWorld()->ServerTravel(_MapNameGM, true);
    //if(_PlayerController_1)
    //    _PlayerController_1->ClientTravel(_MapNameGM, ETravelType::TRAVEL_Absolute);
    //if (_PlayerController_2)
    //    _PlayerController_2->ClientTravel(_MapNameGM, ETravelType::TRAVEL_Absolute);
}

void AGameModeLobby::FindSpawnPoints() {
    if (!_SpawnPoint_1 || !_SpawnPoint_2) {
        for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
            if ((*ActorItr)->GetFName().IsEqual("PlayerStart_1")) {
                _SpawnPoint_1 = *ActorItr;
            }
            else {
                _SpawnPoint_2 = *ActorItr;
            }
        }
    }
}