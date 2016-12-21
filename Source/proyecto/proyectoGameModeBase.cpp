// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "proyectoGameModeBase.h"
#include "CharacterController.h"

AproyectoGameModeBase::AproyectoGameModeBase(const class FObjectInitializer& OI) : Super(OI) {
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT(
        "/Game/Meshes/Skeletons/Prototypes/Player/PlayerCharacter_BP"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;
}