// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "GameModeLobby.h"

#include "NWGameInstance.h"
#include "PlayerControllerLobby.h"


void AGameModeLobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AGameModeLobby, _ServerName);
    DOREPLIFETIME(AGameModeLobby, _MapNameGM);
    DOREPLIFETIME(AGameModeLobby, _MaxPlayers);
}

AGameModeLobby::AGameModeLobby(const class FObjectInitializer& OI) : Super(OI) {
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT(
        "/Game/BluePrints/Characters/PlayerCharacter_BP"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;
    PlayerControllerClass = APlayerControllerLobby::StaticClass();

    bUseSeamlessTravel = true;
    
    _MaxPlayers = 2;
    _ServerName = "";
    _MapNameGM = USettings::Get()->LevelToPlay.GetLongPackageName();

    
}

void AGameModeLobby::InitGame(const FString & MapName, const FString & Options,
                              FString & ErrorMessage) {
    Super::InitGame(MapName, Options, ErrorMessage);

    if (GameSession) GameSession->bRequiresPushToTalk = false;
}

void AGameModeLobby::PostLogin(APlayerController* NewPlayer) {
    Super::PostLogin(NewPlayer);
    
    if (HasAuthority()) {
        UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetGameInstance());
        if (GameInstance) {
            _ServerName = GameInstance->_ServerName;
            _MaxPlayers = GameInstance->_MaxPlayers;
        }

        APlayerControllerLobby* PlayerController = Cast<APlayerControllerLobby>(NewPlayer);
        if (PlayerController) PlayerController->Client_InitialSetup();
    }
}

bool AGameModeLobby::SERVER_SwapCharacter_Validate(APlayerController* PlayerController,
                                                   FPlayerInfo info,
                                                   bool ChangeStatus) { return true; }
void AGameModeLobby::SERVER_SwapCharacter_Implementation(APlayerController* PlayerController,
                                                         FPlayerInfo info,
                                                         bool ChangeStatus) {
    if (!ChangeStatus) {
        if (PlayerController->GetPawn()) PlayerController->GetPawn()->Destroy();

        FTransform transform = FindPlayerStart(PlayerController, info.Name)->GetActorTransform();
        APawn* actor = Cast<APawn>(GetWorld()->SpawnActor(info.CharacterClass, &transform));
        if (actor) PlayerController->Possess(actor);
    }
}

void AGameModeLobby::LaunchGame() {
    GetWorld()->ServerTravel(_MapNameGM, true);
}