// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "GameStatePlay.generated.h"


UCLASS()
class LTMV_API AGameStatePlay : public AGameState {
    GENERATED_BODY()

public:
    bool _StatueWakeUp;

    AGameStatePlay(const class FObjectInitializer& OI);
};
