// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "CharacterController.h"

void ACharacterController::BeginPlay() {
	Super::BeginPlay();

	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It) {
		ACameraActor* _mainCamera = *It;
        if (_mainCamera->GetName() == "PlayerCamera_0") {
            this->SetViewTargetWithBlend(_mainCamera);
        }
	}
}