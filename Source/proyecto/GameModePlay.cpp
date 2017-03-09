// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "GameModePlay.h"

#include "PlayerControllerPlay.h"


void AGameModePlay::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AGameModePlay, _CurrentPlayers);
}

AGameModePlay::AGameModePlay(const class FObjectInitializer& OI) : Super(OI) {
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT(
        "/Game/BluePrints/Characters/PlayerCharacter_BP"));
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
    FTransform transform;
    if (_CurrentPlayers == 0) {
        transform = FindPlayerStart(PlayerController, "host")->GetActorTransform();
    }
    else {
        transform = FindPlayerStart(PlayerController, "guest")->GetActorTransform();
    }
    APawn* actor = Cast<APawn>(GetWorld()->SpawnActor(CharacterClass, &transform));

    if (actor) PlayerController->Possess(actor);
    _CurrentPlayers++;
}