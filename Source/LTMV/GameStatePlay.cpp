// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "GameStatePlay.h"
#include "DoorState.h"
#include "GameModePlay.h"

AGameStatePlay::AGameStatePlay(const class FObjectInitializer& OI) : Super(OI){
    _StatueWakeUp = false;
    _UniqueNetIdArray = {};

	//level
	
	//Coger los PatrolPoint del mapa
	/*
	for (TActorIterator<ATargetPoint> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr->GetName() == "PatrolPoint1") {
			_patrolPoint1 = *ActorItr;
		}
		else if (ActorItr->GetName() == "PatrolPoint2") {
			_patrolPoint2 = *ActorItr;

		}
	}
	*/

	//Coger el enemigo
	/*
	for (TActorIterator<ACharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		/* (ActorItr->GetName() == "EnemyCharacterAnd") {
			ACharacter* _enemy = *ActorItr;
		}
		
	}
	*/
	//Posiciones de los travel point en las puertas
	_puerta1 = FVector(5340.0f, 1250.0f, 0.0f);
	_puerta2 = FVector(6130.0f, 1250.0f, 0.0f);
	_puerta3 = FVector(6130.0f, 850.0f, 0.0f);
	_puerta4 = FVector(5790.0f, 480.0f, 0.0f);
	/*
	//inicio de los travel point
	_patrolPoint1->SetActorLocation(_puerta1);
	_patrolPoint2->SetActorLocation(_puerta2);
	*/

	//Todas las puertas comienzan cerradas
	_doorStates = {};
	_doorStates.Add(false);
	_doorStates.Add(false);
	_doorStates.Add(false);
	_doorStates.Add(false);

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
		if (ActorItr->GetName() == "Puerta1_Blueprint") {
			updateDoor(ActorItr, 0);
		}
		else if (ActorItr->GetName() == "Puerta2_Blueprint") {
			updateDoor(ActorItr, 1);
		}
		else if (ActorItr->GetName() == "Puerta3_Blueprint") {
			updateDoor(ActorItr, 2);
		}
		else if (ActorItr->GetName() == "Puerta4_Blueprint") {
			updateDoor(ActorItr, 3);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Door states: %d,%d,%d,%d"),_doorStates[0], _doorStates[1], _doorStates[2], _doorStates[3]));

	_actualPath = FindPath();
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


TArray<bool> AGameStatePlay::FindPath() {
	//Calcular caminos dependiendo de la posición de las puertas
	//Contar puertas abiertas
	int _openDoors = 0;
	for (int num = 0;num < 4;num++) {
		if (_doorStates[num]) {
			_openDoors++;
		}
	}

	switch (_openDoors) {
		case 0: {
			//4 Posible paths
			//Calcular en qué zona está la IA
			//Zona 0 -> PatrolPoint1=Puerta1; Patrolpoint2=Puerta2
			//Zona 1 -> PatrolPoint1=Puerta2; Patrolpoint2=Puerta3
			//Zona 2 -> PatrolPoint1=Puerta3; Patrolpoint2=Puerta4
			//Zona 3 -> PatrolPoint1=Puerta1; Patrolpoint2=Puerta4
			break;
		}
		case 1: {
			//3 posibles caminos dependiendo de la puerta abierta
			//Calcular en qué zona está la IA
			/*	Si Puerta1 abierta -> 
					IA en Zona 0 o 3-> PatrolPoint1=Puerta4; PatrolPoint=Puerta2
					IA en Zona 1 -> PatrolPoint1=Puerta2; PatrolPoint2=Puerta3
					IA en Zona 2 -> PatrolPoint1=Puerta3; PatrolPoint2=Puerta4
				Si Puerta2 abierta -> 
					IA en Zona 0 o 1 -> PatrolPoint=Puerta1; PatrolPoint2=Puerta2
					IA en Zona 2 -> PatrolPoint1=Puerta3; PatrolPoint2=Puerta4
					IA en Zona 3 -> PatrolPoint1=Puerta1; PatrolPoint2=Puerta4
				Si Puerta3 abierta -> 
					IA en Zona 1 o 2-> PatrolPoint1=Puerta2; PatrolPoint2=Puerta4
					IA en Zona 0 -> PatrolPoint1=Puerta1; Patrolpoint2=Puerta2
					IA en Zona 3 -> PatrolPoint1=Puerta1; PatrolPoint2=Puerta4
				*/
			break;
		}
		case 2: {
			//2 posibles caminos dependiendo de las puertas
			/*	Si Puerta1 y Puerta2 abiertas ->
					IA en Zona 0, 1 o 3 -> PatrolPoint1=Puerta4; PatrolPoint2=Puerta3
					IA en Zona 2 -> PatrolPoint1=Puerta3; PatrolPoint2=Puerta4
				Si Puerta1 y Puerta3 abiertas ->
					IA en Zona 0 o 3 -> PatrolPoint1=Puerta4; PatrolPoint2=Puerta2
					IA en Zona 1 o 2 -> PatrolPoint1=Puerta2; PatrolPoint2=Puerta4
				Si Puerta1 y Puerta4 abiertas ->
					IA en Zona 0, 2 o 3 -> PatrolPoint1=Puerta2; PatrolPoint2=Puerta3
					IA en Zona 1 -> PatrolPoint1=Puerta2; PatrolPoint2=Puerta3
				Si Puerta2 y Puerta3 abiertas ->
					IA en Zona 0, 1 o 2 -> PatrolPoint1=Puerta1; PatrolPoint2=Puerta4
					IA en Zona 3 -> PatrolPoint1=Puerta1; PatrolPoint2=Puerta4
				Si Puerta2 y Puerta4 abiertas ->
					IA en Zona 0 o 1 -> PatrolPoint1=Puerta1; PatrolPoint2=Puerta3
					IA en Zona 2 o 3 -> PatrolPoint1=Puerta1; PatrolPoint2=Puerta3
				Si Puerta3 y Puerta4 abiertas ->
					IA en Zona 0 -> PatrolPoint1=Puerta1; PatrolPoint2=Puerta2
					IA en Zona 1,2 o 3 -> PatrolPoint1=Puerta1; PatrolPoint2=Puerta2
			*/
			break;
		}
		case 3: {
			//1 posible camino
			//¿Como hacer?
			break;
		}
		case 4: {
			//No hay caminos
			// Dar vueltas!!!!!
			break;
		}
	}

	return _actualPath;
}