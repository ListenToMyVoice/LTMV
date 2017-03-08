// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "GameModePlay.h"

#include "PlayerControllerPlay.h"


void AGameModePlay::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AGameModePlay, _CurrentPlayers);
    DOREPLIFETIME(AGameModePlay, _SpawnPoint_1);
    DOREPLIFETIME(AGameModePlay, _SpawnPoint_2);
}

AGameModePlay::AGameModePlay(const class FObjectInitializer& OI) : Super(OI) {
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT(
        "/Game/BluePrints/PlayerCharacter_BP"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;
    PlayerControllerClass = APlayerControllerPlay::StaticClass();

    _CurrentPlayers = 0;
}

bool AGameModePlay::SERVER_RespawnPlayer_Validate(APlayerController* PlayerController,
                                                   TSubclassOf<ACharacter> CharacterClass) {
    return true;
}
void AGameModePlay::SERVER_RespawnPlayer_Implementation(APlayerController* PlayerController,
                                                        TSubclassOf<ACharacter> CharacterClass) {
    if (PlayerController->GetPawn()) {
        PlayerController->GetPawn()->Destroy();
    }
    FindSpawnPoints();
    FTransform transform;
    if (_CurrentPlayers == 0) {
        transform = _SpawnPoint_1->GetActorTransform();
    }
    else {
        transform = _SpawnPoint_2->GetActorTransform();
    }
    APawn* actor = Cast<APawn>(GetWorld()->SpawnActor(CharacterClass, &transform));

    if (actor) PlayerController->Possess(actor);
    _CurrentPlayers++;
    ULibraryUtils::Log(CharacterClass.GetDefaultObject()->GetFName().ToString());
}

void AGameModePlay::FindSpawnPoints() {
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