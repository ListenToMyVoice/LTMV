// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "GameStateLTMV.generated.h"

class AMusicActor;

UCLASS()
class PROYECTO_API AGameStateLTMV : public AGameState {
    GENERATED_BODY()

private:
    AMusicActor* _musicActor;

public:
    AGameStateLTMV(const class FObjectInitializer& OI);
    virtual void BeginPlay() override;
};
