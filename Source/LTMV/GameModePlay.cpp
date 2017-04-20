// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "GameModePlay.h"

#include "PlayerControllerPlay.h"
#include "GameStatePlay.h"


AGameModePlay::AGameModePlay(const class FObjectInitializer& OI) : Super(OI) {
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT(
        "/Game/BluePrints/Characters/PlayerCharacter_BP"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;
    
    static ConstructorHelpers::FClassFinder<ASpectatorPawn> SpectatorClassFinder(TEXT(
        "/Game/BluePrints/Characters/SpectadorPawnPlay"));
    SpectatorClass = SpectatorClassFinder.Class;
    
    PlayerControllerClass = APlayerControllerPlay::StaticClass();
    
    GameStateClass = AGameStatePlay::StaticClass();
}

void AGameModePlay::InitGame(const FString & MapName, const FString & Options,
                             FString & ErrorMessage) {
    Super::InitGame(MapName, Options, ErrorMessage);

    if (GameSession) GameSession->bRequiresPushToTalk = true;
}

bool AGameModePlay::SERVER_RespawnPlayer_Validate(APlayerControllerPlay* PlayerController,
                                                  FPlayerInfo info) {
    return true;
}

void AGameModePlay::SERVER_RespawnPlayer_Implementation(APlayerControllerPlay* PlayerController,
                                                        FPlayerInfo info) {
    if (PlayerController->GetPawn()) PlayerController->GetPawn()->Destroy();

    FTransform transform = FindPlayerStart(PlayerController, info.Name)->GetActorTransform();
    APawn* actor = Cast<APawn>(GetWorld()->SpawnActor(info.CharacterClass, &transform));
    if (actor) {
        PlayerController->Possess(actor);
        PlayerController->AfterPossessed();

        if (!_HostController) _HostController = PlayerController;
        else _GuestController = PlayerController;
    }
}

bool AGameModePlay::SERVER_PlayerDead_Validate(AController* PlayerController) {
    return true;
}

void AGameModePlay::SERVER_PlayerDead_Implementation(AController* Controller) {
    APlayerControllerPlay* PlayerController = Cast<APlayerControllerPlay>(Controller);
    if (PlayerController) {
        if (_HostController) _HostController->CLIENT_Dead(PlayerController->PlayerState->UniqueId);
        if (_GuestController) _GuestController->CLIENT_Dead(PlayerController->PlayerState->UniqueId);

        PlayerController->ChangeState(NAME_Spectating);
        PlayerController->CLIENT_GotoState(NAME_Spectating);
        //PlayerController->GameHasEnded(PlayerController->_MenuActor, false);
    }
}