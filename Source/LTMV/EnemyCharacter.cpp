// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "EnemyCharacter.h"

#include "EnemyController.h"

AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& OI) : Super(OI) {
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    bReplicateMovement = true;

    AIControllerClass = AEnemyController::StaticClass();

    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
    AutoPossessAI = EAutoPossessAI::Disabled;
    _SightRadius = 500.0f;
    _LoseSightRadius = 1000.0f;
    _VisionAngleDegrees = 90.0f;
	_HearingRange = 1500.0f;
    _Damage = 1;
}

void AEnemyCharacter::BeginPlay() {
    Super::BeginPlay();
}

void AEnemyCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
				 class AController* EventInstigator, class AActor* DamageCauser) {
	ULibraryUtils::Log(FString::Printf(TEXT("Me han dado")), 0, 60);
	/*The enemy doesn't receive damage*/
	return 0.0f;
}