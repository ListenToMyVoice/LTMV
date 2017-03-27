// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class LTMV_API AEnemyCharacter : public ACharacter {
    GENERATED_BODY()

public:
    AEnemyCharacter(const FObjectInitializer& OI);

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};
