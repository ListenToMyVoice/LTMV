// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "GameStatePlay.generated.h"


UCLASS()
class LTMV_API AGameStatePlay : public AGameState {
    GENERATED_BODY()

public:
    bool _StatueWakeUp;
    TArray<TSharedPtr<const FUniqueNetId>> _UniqueNetIdArray;

    AGameStatePlay(const class FObjectInitializer& OI);

	FVector _puerta1;
	FVector _puerta2;
	FVector _puerta3;
	FVector _puerta4;

	FVector _point1;
	FVector _point2;

	TArray<bool> _doorStates;
	TArray<bool> _actualPath;
	TArray<bool> _beforePath;
	TArray<bool> _PatrolPoints;

	void updateDoors();

private:
	void updateDoor(TActorIterator<AStaticMeshActor> _door, int value);
};
