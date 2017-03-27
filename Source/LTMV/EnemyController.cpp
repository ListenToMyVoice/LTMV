// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "EnemyController.h"


AEnemyController::AEnemyController(const FObjectInitializer& OI) : Super(OI) {
    // Setup the perception component
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
    sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    PerceptionComponent->ConfigureSense(*sightConfig);
    PerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
    PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAIControllerUnit::SenseStuff);
}

void AEnemyController::WakeUp(UBehaviorTree* Tree) {
    RunBehaviorTree(Tree);
}