// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "GameModePlay.h"

#include "PlayerControllerPlay.h"


AGameModePlay::AGameModePlay(const class FObjectInitializer& OI) : Super(OI) {
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT(
        "/Game/BluePrints/Characters/PlayerCharacter_BP"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;
    PlayerControllerClass = APlayerControllerPlay::StaticClass();

    if (GameSession) GameSession->bRequiresPushToTalk = true;
}

void AGameModePlay::InitGame(const FString & MapName, const FString & Options,
                              FString & ErrorMessage) {
    Super::InitGame(MapName, Options, ErrorMessage);

    if (GameSession) GameSession->bRequiresPushToTalk = true;
}


bool AGameModePlay::SERVER_RespawnPlayer_Validate(APlayerController* PlayerController,
                                                  FPlayerInfo info) {
    return true;
}

void AGameModePlay::SERVER_RespawnPlayer_Implementation(APlayerController* PlayerController,
                                                        FPlayerInfo info) {
    if (PlayerController->GetPawn()) PlayerController->GetPawn()->Destroy();
    
    FTransform transform = FindPlayerStart(PlayerController, info.Name)->GetActorTransform();
    APawn* actor = Cast<APawn>(GetWorld()->SpawnActor(info.CharacterClass, &transform));
    if (actor) PlayerController->Possess(actor);
}