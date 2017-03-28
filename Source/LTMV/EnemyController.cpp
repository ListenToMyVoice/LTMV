// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "EnemyController.h"

#include "EnemyCharacter.h"


AEnemyController::AEnemyController(const FObjectInitializer& OI) : Super(OI) {
    _SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    _HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

    _PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
    _PerceptionComp->ConfigureSense(*_SightConfig);
    _PerceptionComp->SetDominantSense(_SightConfig->GetSenseImplementation());
    _PerceptionComp->ConfigureSense(*_HearingConfig);
    _PerceptionComp->SetDominantSense(_HearingConfig->GetSenseImplementation());
    _PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::SenseStuff);
}

void AEnemyController::Possess(APawn* InPawn) {
    AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(InPawn);
    if (EnemyCharacter) {
        Super::Possess(InPawn);

        ApplySenses(EnemyCharacter->_SightRange, EnemyCharacter->_HearingRange);
        UAIPerceptionSystem::RegisterPerceptionStimuliSource(this,
                                                             _SightConfig->GetSenseImplementation(),
                                                             GetControlledPawn());
    }
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Possess");
}

void AEnemyController::ApplySenses(float SightRange, float HearingRange) {
    /* Sight */
    _SightConfig->SightRadius = SightRange;
    _SightConfig->LoseSightRadius = SightRange + 20.0f;
    _SightConfig->PeripheralVisionAngleDegrees = 360.0f;
    _SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    _SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    _SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    _PerceptionComp->ConfigureSense(*_SightConfig);
    _PerceptionComp->SetDominantSense(_SightConfig->GetSenseImplementation());

    /* Hearing */
    _HearingConfig->HearingRange = HearingRange;
    _HearingConfig->LoSHearingRange = HearingRange + 20.0f;
    _HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    _HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    _HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    _PerceptionComp->ConfigureSense(*_HearingConfig);
    _PerceptionComp->SetDominantSense(_HearingConfig->GetSenseImplementation());

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ApplySenses");
}

void AEnemyController::SenseStuff(TArray<AActor*> testActors) {
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "SenseStuff");
}

//void AEnemyController::WakeUp(UBehaviorTree* Tree) {
//    RunBehaviorTree(Tree);
//}
