// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnemyController.generated.h"


UCLASS()
class LTMV_API AEnemyController : public AAIController {
    GENERATED_BODY()

public:
    AEnemyController(const FObjectInitializer& OI);

    UFUNCTION(BlueprintCallable, Category = "AI")
    void WakeUp(UBehaviorTree* Tree);
};
