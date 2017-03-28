// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "EnemyCharacter.h"

#include "EnemyController.h"

AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& OI) : Super(OI) {
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    bReplicateMovement = true;

    AIControllerClass = AEnemyController::StaticClass();

    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
    AutoPossessAI = EAutoPossessAI::Disabled;
    _SightRange = 20.0f;
    _HearingRange = 20.0f;
}

void AEnemyCharacter::BeginPlay() {
    Super::BeginPlay();
}

void AEnemyCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}