// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Perception/AISenseConfig_Sight.h"
//#include "Perception/AISenseConfig_Hearing.h"

#include "AIController.h"
#include "EnemyController.generated.h"


UCLASS()
class LTMV_API AEnemyController : public AAIController {
    GENERATED_BODY()

public:
    UAISenseConfig_Sight* _SightConfig;
    //UAISenseConfig_Hearing* _HearingConfig;

    AEnemyController(const FObjectInitializer& OI);

    //UFUNCTION(BlueprintCallable, Category = "AI")
    //void WakeUp(UBehaviorTree* Tree);

    virtual void Possess(APawn* InPawn) override;

protected:
    UFUNCTION()
    void SenseStuff(TArray<AActor*> Actors);

private:
    void ApplySenses(float SightRange, float HearingRange);
};
