// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "GameModePlay.h"

#include "PlayerControllerPlay.h"
#include "PlayerCharacter.h"
#include "GameStatePlay.h"
#include "NWGameInstance.h"
#include "PlayerSpectator.h"
#include "FPCharacter.h"
#include "Inventory.h"


AGameModePlay::AGameModePlay(const class FObjectInitializer& OI) : Super(OI) {
    DefaultPawnClass = nullptr;
    SpectatorClass = APlayerSpectator::StaticClass();
    PlayerControllerClass = APlayerControllerPlay::StaticClass();
    GameStateClass = AGameStatePlay::StaticClass();

    bUseSeamlessTravel = true;
	_MapNameGM = USettings::Get()->LevelToPlay.GetLongPackageName();
	_MapEndGameGM = USettings::Get()->EndLevel.GetLongPackageName();

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
	//Cogemos el playercharacter actual
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
	UNWGameInstance* GI = Cast<UNWGameInstance>(GetWorld()->GetGameInstance());
	AGameStatePlay* GameState = Cast<AGameStatePlay>(GetWorld()->GetGameState());

	//para el VRCharacter
	if (GI && GI->_IsVRMode) {


	}
	//para el fpcharacter
	else {

		//Cerrar inventario si está abierto
		AFPCharacter* _Player = Cast<AFPCharacter>(PlayerController->GetPawn());
		_Player->HideInventory();

		//Drop y delete del mundo de los items almacenados en el inventario
		UInventory* _inventory = _Player->GetInventory();
		TArray<AActor*> _items = _inventory->GetItemsArray();

		for (AActor* item : _items) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("-- DROP DE ITEM: %s"), *item->GetName()));
			PlayerCharacter->SERVER_Drop(item, 4);
			GameState->DeleteAsset(item);
		}
	}
	//Destruir el pawn actual
	if (PlayerController->GetPawn()) PlayerController->GetPawn()->Destroy();

	//Crear un nuevo pawn
	FTransform transform = FindPlayerStart(PlayerController, info.Name)->GetActorTransform();
	APawn* actor = Cast<APawn>(GetWorld()->SpawnActor(info.CharacterClass, &transform));
	if (actor) {
		PlayerController->Possess(actor);
		if (!_HostController) _HostController = PlayerController;
		else _GuestController = PlayerController;

		//Afterpossesed del nuevo actor
		PlayerController->AfterPossessed(true);
		//Nuevo character con ese actor
		PlayerCharacter = Cast<APlayerCharacter>(actor);
		//para el VRCharacter
		if (GI && GI->_IsVRMode) {
		}
		//para el FPCharacter
		else {
			// SPAWNEAMOS UN NUEVO WALKIE Y UNA NUEVA LINTERNA EN EL CHARACTER
			FActorSpawnParameters SpawnParams;
			FVector location = FVector(0.0f, 0.0f, 0.0f);
			FRotator rotation = FRotator(0.0f, 0.0f, 0.0f);
			AActor* Walkie = GetWorld()->SpawnActor<AActor>(WalkieBlueprint, location, rotation, SpawnParams);
			if (Walkie) {
				PlayerCharacter->TakeDropRight_Respawn(Walkie);
				//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("**** NUEVO WALKIE")));
			}
			// LINTERNA
			AActor* Linterna = GetWorld()->SpawnActor<AActor>(LinternaBlueprint, location, rotation, SpawnParams);
			if (Linterna) {
				PlayerCharacter->TakeDropRight_Respawn(Linterna);
				//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("**** NUEVA LINTERNA")));
			}
		}
	}

	/*
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
	*/
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
			SERVER_RespawnPlayerAfterDeath(PlayerController, GameInstance->_PlayerInfoSaved);
			AGameStatePlay* GameState = Cast<AGameStatePlay>(GetWorld()->GetGameState());
			GameState->ResetLevel();
		 }
        if (_GuestController == PlayerController) {
        }
    }
}


void AGameModePlay::EndGame() {
	GetWorld()->ServerTravel(_MapEndGameGM, true);
}
