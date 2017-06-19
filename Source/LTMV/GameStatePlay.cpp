// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "GameStatePlay.h"
#include "DoorState.h"
#include "GameModePlay.h"

AGameStatePlay::AGameStatePlay(const class FObjectInitializer& OI) : Super(OI){
    _StatueWakeUp = false;
    _UniqueNetIdArray = {};

	//level
	_puerta1 = FVector(5340.0f, 1250.0f, 0.0f);
	_puerta2 = FVector(6130.0f, 1250.0f, 0.0f);
	_puerta3 = FVector(6130.0f, 850.0f, 0.0f);
	_puerta4 = FVector(5790.0f, 480.0f, 0.0f);

	//Todas las puertas comienzan abiertas
	_doorStates = {};
	_doorStates.Add(true);
	_doorStates.Add(true);
	_doorStates.Add(true);
	_doorStates.Add(true);

	//Path comienza sólo en la zona 0
	_beforePath = {};
	_actualPath = {};
	_beforePath.Add(true);
	_beforePath.Add(false);
	_beforePath.Add(false);
	_beforePath.Add(false);
	_actualPath = _beforePath;

	//Patrol points empiezan en la puerta 1 y en la puerta 2
	_PatrolPoints = {};
	_PatrolPoints.Add(true);
	_PatrolPoints.Add(true);
	_PatrolPoints.Add(false);
	_PatrolPoints.Add(false);


}

void AGameStatePlay::updateDoors() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("GameState: UpdateDoors! ")));
	//pick values from doors in the world
	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetName() == "Puerta1") {
			updateDoor(ActorItr, 0);
		}
		if (ActorItr->GetName() == "Puerta2") {
			updateDoor(ActorItr, 1);
		}
		if (ActorItr->GetName() == "Puerta3") {
			updateDoor(ActorItr, 2);
		}
		if (ActorItr->GetName() == "Puerta4") {
			updateDoor(ActorItr, 3);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Door states: %d,%d,%d,%d"),_doorStates[0], _doorStates[1], _doorStates[2], _doorStates[3]));

}
void AGameStatePlay::updateDoor(TActorIterator<AStaticMeshActor> _door, int value) {
	UDoorState* Door = Cast<UDoorState>(_door->GetComponentByClass(UDoorState::StaticClass()));
	if (Door->GetState() == 1) {
		_doorStates[value] = true;
	}
	else if (Door->GetState() == 0) {
		_doorStates[value] = false;
	}
}