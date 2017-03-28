// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "EnemyController.h"


AEnemyController::AEnemyController(const FObjectInitializer& OI) : Super(OI) {
    _SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    _HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

    _PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
    _PerceptionComp->ConfigureSense(*Cast<UAISenseConfig>(_SightConfig));
    _PerceptionComp->SetDominantSense(_SightConfig->GetSenseImplementation());
    _PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::SenseSight);
    _PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::SenseHearing);
}

void AEnemyController::Possess(APawn* InPawn) {
    Super::Possess(InPawn);
    ApplySenses();

    UAIPerceptionSystem::RegisterPerceptionStimuliSource(this,
                                                         _SightConfig->GetSenseImplementation(),
                                                         GetControlledPawn());
}

void AEnemyController::ApplySenses() {
    /* Sight */
    //_SightConfig->SightRadius = InPawn->sightRange;
    //_SightConfig->LoseSightRadius = (InPawn->sightRange + 20.0f);
    _SightConfig->PeripheralVisionAngleDegrees = 360.0f;
    _SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    _SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    _SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    _PerceptionComp->ConfigureSense(*_SightConfig);

    /* Hearing */
    //_HearingConfig->HearingRange = InPawn->HearingRange;
    //_HearingConfig->LoSHearingRange = (InPawn->HearingRange + 20.0f);
    _HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    _HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    _HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    _PerceptionComp->ConfigureSense(*_HearingConfig);
}

void AEnemyController::SenseSight(TArray<AActor*> testActors) {
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I SEE you!");
}

void AEnemyController::SenseHearing(TArray<AActor*> testActors) {
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I HEAR you!");
}

//void AEnemyController::WakeUp(UBehaviorTree* Tree) {
//    RunBehaviorTree(Tree);
//}
