// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "proyectoGameMode.h"
#include "CharacterController.h"
#include "GameStateLTMV.h"

AproyectoGameMode::AproyectoGameMode(const class FObjectInitializer& OI) : Super(OI) {
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT(
        "/Game/Meshes/Skeletons/Prototypes/Player/PlayerCharacter_BP"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    PlayerControllerClass = ACharacterController::StaticClass();
    GameStateClass = AGameStateLTMV::StaticClass();
}