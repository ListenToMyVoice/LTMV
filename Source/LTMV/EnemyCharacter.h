// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class LTMV_API AEnemyCharacter : public ACharacter {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Settings")
    float _SightRadius;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
    float _LoseSightRadius;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
    float _VisionAngleDegrees;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
    class UBehaviorTree* _BehaviourTree;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    int _Damage;

    AEnemyCharacter(const FObjectInitializer& OI);

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};
