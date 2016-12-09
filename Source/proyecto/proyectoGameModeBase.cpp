// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "proyectoGameModeBase.h"
#include "CharacterController.h"

AproyectoGameModeBase::AproyectoGameModeBase(const class FObjectInitializer& PCIP) : Super(PCIP) {
    PlayerControllerClass = ACharacterController::StaticClass();

    static ConstructorHelpers::FObjectFinder<UClass> PlayerPawnBPClass(
        TEXT("Class'/Game/Character/PlayerCharacter_BP.PlayerCharacter_BP_C'")
    );

    if (PlayerPawnBPClass.Object != NULL) {
        DefaultPawnClass = PlayerPawnBPClass.Object;
    }
}