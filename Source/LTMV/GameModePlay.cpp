// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "GameModePlay.h"

#include "PlayerControllerPlay.h"
#include "PlayerCharacter.h"
#include "GameStatePlay.h"
#include "NWGameInstance.h"
#include "PlayerSpectator.h"


AGameModePlay::AGameModePlay(const class FObjectInitializer& OI) : Super(OI) {
    DefaultPawnClass = nullptr;
    SpectatorClass = APlayerSpectator::StaticClass();
    PlayerControllerClass = APlayerControllerPlay::StaticClass();
    GameStateClass = AGameStatePlay::StaticClass();

    bUseSeamlessTravel = true;
	_MapNameGM = USettings::Get()->LevelToPlay.GetLongPackageName();
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

		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(actor);

		PlayerCharacter->AfterPossessed(true,true);
		/*
        PlayerController->AfterPossessed();
		*/
        if (!_HostController) _HostController = PlayerController;
        else _GuestController = PlayerController;
    }
}

bool AGameModePlay::SERVER_PlayerDead_Validate(AController* PlayerController) {
    return true;
}

void AGameModePlay::SERVER_PlayerDead_Implementation(AController* Controller) {

	UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetGameInstance());
    APlayerControllerPlay* PlayerController = Cast<APlayerControllerPlay>(Controller);
    if (PlayerController) {
        //PlayerController->ChangeState(NAME_Spectating);

        if (_HostController == PlayerController) {
			SERVER_RespawnPlayer(PlayerController,GameInstance->_PlayerInfoSaved);
			/*
			//PlayAgain();
            _HostController->CLIENT_Dead();
            if(_GuestController) _GuestController->CLIENT_ShowMenu();
			//if (_GuestController) _GuestController->CLIENT_GotoState(NAME_Spectating);
			*/
        }
        if (_GuestController == PlayerController) {
			SERVER_RespawnPlayer(PlayerController, GameInstance->_PlayerInfoSaved);
			/*
			//PlayAgain();
            _GuestController->CLIENT_GotoState(NAME_Spectating);
            _HostController->CLIENT_ShowMenu();
			*/
        }
    }
}


void AGameModePlay::PlayAgain() {
	RestartGame();
	//GetWorld()->ServerTravel(_MapNameGM, true);
	//GetWorld()->ServerTravel(_MapNameGM, true);
	//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
