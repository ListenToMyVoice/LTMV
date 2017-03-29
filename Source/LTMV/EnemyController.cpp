// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "EnemyController.h"

#include "EnemyCharacter.h"
#include "Perception/AIPerceptionComponent.h"

AEnemyController::AEnemyController(const FObjectInitializer& OI) : Super(OI) {
    _SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    //_HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
    //PerceptionComponent->ConfigureSense(*_SightConfig);
    //PerceptionComponent->SetDominantSense(_SightConfig->GetSenseImplementation());
    //PerceptionComponent->ConfigureSense(*_HearingConfig);
    //PerceptionComponent->SetDominantSense(_HearingConfig->GetSenseImplementation());
    //PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::SenseStuff);
}

void AEnemyController::Possess(APawn* InPawn) {
    AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(InPawn);
    if (EnemyCharacter) {
        Super::Possess(InPawn);

        ApplySenses(EnemyCharacter->_SightRange, EnemyCharacter->_HearingRange);
        //UAIPerceptionSystem::RegisterPerceptionStimuliSource(this,
        //                                                     _SightConfig->GetSenseImplementation(),
        //                                                     GetPawn());
        //UAIPerceptionSystem::RegisterPerceptionStimuliSource(this,
        //                                                     _HearingConfig->GetSenseImplementation(),
        //                                                     GetPawn());
    }
}

void AEnemyController::ApplySenses(float SightRange, float HearingRange) {
    /* Sight */
    _SightConfig->SightRadius = SightRange;
    _SightConfig->LoseSightRadius = SightRange + 20.0f;
    _SightConfig->PeripheralVisionAngleDegrees = 360.0f;
    _SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    _SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    _SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    PerceptionComponent->ConfigureSense(*_SightConfig);
    PerceptionComponent->SetDominantSense(_SightConfig->GetSenseImplementation());

    PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::PerceptionUpdated);
    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::TargetPerceptionUpdated);

    /* Hearing */
    //_HearingConfig->HearingRange = HearingRange;
    //_HearingConfig->LoSHearingRange = HearingRange + 20.0f;
    //_HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    //_HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    //_HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    //PerceptionComponent->ConfigureSense(*_HearingConfig);
    //PerceptionComponent->SetDominantSense(_HearingConfig->GetSenseImplementation());

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ApplySenses");
}

void AEnemyController::PerceptionUpdated(TArray<AActor*> Actors) {
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "PerceptionUpdated");
}

void AEnemyController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) {
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "OnTargetPerceptionUpdated");
}

//void AEnemyController::WakeUp(UBehaviorTree* Tree) {
//    RunBehaviorTree(Tree);
//}
