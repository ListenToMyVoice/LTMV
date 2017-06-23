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

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/BluePrints/Assets/walkie.walkie'"));
	if (ItemBlueprint.Object) {
		WalkieBlueprint = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint2(TEXT("Blueprint'/Game/BluePrints/Assets/LinternaFINAL.LinternaFINAL'"));
	if (ItemBlueprint2.Object) {
		LinternaBlueprint = (UClass*)ItemBlueprint2.Object->GeneratedClass;
	}
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
		PlayerController->AfterPossessed(false);

		if (!_HostController) _HostController = PlayerController;
		else _GuestController = PlayerController;
	}
}


/* ****** RESPAWN AFTER DEATH ********/
bool AGameModePlay::SERVER_RespawnPlayerAfterDeath_Validate(APlayerControllerPlay* PlayerController,
	FPlayerInfo info) {
	return true;
}

void AGameModePlay::SERVER_RespawnPlayerAfterDeath_Implementation(APlayerControllerPlay* PlayerController,
	FPlayerInfo info) {
	if (PlayerController->GetPawn()) PlayerController->GetPawn()->Destroy();

	FTransform transform = FindPlayerStart(PlayerController, info.Name)->GetActorTransform();
	APawn* actor = Cast<APawn>(GetWorld()->SpawnActor(info.CharacterClass, &transform));
	if (actor) {
		PlayerController->Possess(actor);

		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(actor);
		PlayerController->AfterPossessed(true);

		
		//Eliminamos la información en los player de los objetos del jugador muerto del inventario
		for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			if (ActorItr->GetName() == "walkie_laberinto") {
				PlayerCharacter->SERVER_Drop(*ActorItr, 4);
			}
		}
		for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			if (ActorItr->GetName() == "Linterna_laberinto") {
				PlayerCharacter->SERVER_Drop(*ActorItr, 4);
			}
		}
		// WALKIE
		FActorSpawnParameters SpawnParams;
		FVector location= FVector(0.0f, 0.0f, 0.0f);
		FRotator rotation = FRotator(0.0f, 0.0f, 0.0f);
		AActor* Walkie = GetWorld()->SpawnActor<AActor>(WalkieBlueprint, location, rotation, SpawnParams);
		if (Walkie) {
			PlayerCharacter->TakeDropRight_Respawn(Walkie);
		}

		// LINTERNA
		AActor* Linterna = GetWorld()->SpawnActor<AActor>(LinternaBlueprint, location, rotation, SpawnParams);
		if (Walkie) {
			PlayerCharacter->TakeDropRight_Respawn(Linterna);
		}

		if (!_HostController) _HostController = PlayerController;
		else _GuestController = PlayerController;
	}
}



/*********death**************/
bool AGameModePlay::SERVER_PlayerDead_Validate(AController* PlayerController) {
	return true;
}
void AGameModePlay::SERVER_PlayerDead_Implementation(AController* Controller) {

	UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetGameInstance());
    APlayerControllerPlay* PlayerController = Cast<APlayerControllerPlay>(Controller);
    if (PlayerController) {
        if (_HostController == PlayerController) {
			AGameStatePlay* GameState = Cast<AGameStatePlay>(GetWorld()->GetGameState());
			GameState->ResetLevel();
			SERVER_RespawnPlayerAfterDeath(PlayerController,GameInstance->_PlayerInfoSaved);
        }
        if (_GuestController == PlayerController) {
        }
    }
}


void AGameModePlay::PlayAgain() {
	RestartGame();
	//GetWorld()->ServerTravel(_MapNameGM, true);
	//GetWorld()->ServerTravel(_MapNameGM, true);
	//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
