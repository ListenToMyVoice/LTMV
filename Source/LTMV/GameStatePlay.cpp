// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "GameStatePlay.h"
#include "DoorState.h"
#include "GameModePlay.h"

AGameStatePlay::AGameStatePlay(const class FObjectInitializer& OI) : Super(OI){
    _StatueWakeUp = false;
    _UniqueNetIdArray = {};

	//level
	
	//Coger el enemigo
	/*
		
	}
	*/
	//Posiciones de los travel point en las puertas
	_puerta1_0 = FVector(5340.0f, 1370.0f, 0.0f);
	_puerta1_1 = FVector(5340.0f, 1150.0f, 0.0f);
	_puerta2_0 = FVector(6000.0f, 1440.0f, 0.0f);
	_puerta2_1 = FVector(6270.0f, 1440.0f, 0.0f);
	_puerta3_0 = FVector(6270.0f, 850.0f, 0.0f);
	_puerta3_1 = FVector(6020.0f, 850.0f, 0.0f);
	_puerta4_0 = FVector(5910.0f, 490.0f, 0.0f);
	_puerta4_1 = FVector(5650.0f, 490.0f, 0.0f);
	// Posiciones de los travel point intermedios cuando son en medio de zonas
	_point1_2 = FVector(5660.0f, 1430.0f, 0.0f);
	_point2_3 = FVector(6290.0f, 1130.0f, 0.0f);
	_point3_4 = FVector(5970.0f, 630.0f, 0.0f);
	_point4_1 = FVector(5220.0f, 240.0f, 0.0f);

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
	//first time here
	if (!_patrolPoint1 && !_patrolPoint2) {
		//Coger los PatrolPoint del mapa
		for (TActorIterator<ATargetPoint> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			if (ActorItr->GetName() == "PatrolPoint1") {
				_patrolPoint1 = *ActorItr;
			}
			else if (ActorItr->GetName() == "PatrolPoint2") {
				_patrolPoint2 = *ActorItr;
			}
			else if (ActorItr->GetName() == "PatrolPoint3") {
				_patrolPoint3= *ActorItr;
			}
		}
		//inicio de los travel point
		/*
		_patrolPoint1->SetActorLocation(_puerta1_0);
		_patrolPoint2->SetActorLocation(_puerta2_0);
		*/

		//Coger al enemigo
		for (TActorIterator<ACharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			if (ActorItr->GetName() == "EnemyCharacterAnd") {
				_enemy = *ActorItr;
			}
		}
	}

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
			//Zona 0 -> PatrolPoint1=Puerta1_0; Patrolpoint2=Puerta2_0; Patrolpoint3=Point1-2
			if (_actualZone==0){
				UpdatePatrolPoints(_puerta1_0, _puerta2_0, _point1_2);
			}
			if (_actualZone == 1) {
				//Zona 1 -> PatrolPoint1=Puerta2_1; Patrolpoint2=Puerta3_0
				UpdatePatrolPoints(_puerta2_1, _puerta3_0, _point2_3);
			}
			if (_actualZone == 2) {
				//Zona 2 -> PatrolPoint1=Puerta3_1; Patrolpoint2=Puerta4_0
				UpdatePatrolPoints(_puerta2_1, _puerta4_0, _point3_4);
			}
			if (_actualZone == 3) {
				//Zona 3 -> PatrolPoint1=Puerta1_1; Patrolpoint2=Puerta4_1
				UpdatePatrolPoints(_puerta1_1, _puerta4_1, _point4_1);
			}
			break;
		}
		case 1: {
			//3 posibles caminos dependiendo de la puerta abierta
			/* Si Puerta1 abierta ->*/
			if (_doorStates[0]) {
				if (_actualZone == 0 || _actualZone == 3) {
					UpdatePatrolPoints(_puerta4_1, _puerta2_0, _puerta1_1);
				}
				else if (_actualZone == 1) {
					UpdatePatrolPoints(_puerta2_1, _puerta3_0, _point2_3);
				}
				else if (_actualZone == 2) {
					UpdatePatrolPoints(_puerta3_1, _puerta4_0, _point3_4);
				}
			}
			/*Si Puerta2 abierta ->*/
			else if (_doorStates[1]) {
				if (_actualZone == 0 || _actualZone == 1) {
					UpdatePatrolPoints(_puerta1_0, _puerta3_0, _puerta2_0);
				}
				else if (_actualZone == 2) {
					UpdatePatrolPoints(_puerta3_1, _puerta4_0, _point3_4);
				}
				else if (_actualZone == 3) {
					UpdatePatrolPoints(_puerta1_1, _puerta4_1, _point4_1);
				}
			}
			/* Si Puerta3 abierta -> */
			else if (_doorStates[2]) {
				if (_actualZone == 2 || _actualZone == 1) {
					UpdatePatrolPoints(_puerta2_1, _puerta4_0, _puerta3_0);
				}
				else if (_actualZone == 3) {
					UpdatePatrolPoints(_puerta1_1, _puerta4_1, _point4_1);
				}
				else if (_actualZone == 0) {
					UpdatePatrolPoints(_puerta1_0, _puerta2_0, _point1_2);
				}
			}

			/* Si Puerta4 abierta -> */
			else if (_doorStates[3]) {
				if (_actualZone == 2 || _actualZone == 3) {
					UpdatePatrolPoints(_puerta1_1, _puerta3_1, _puerta4_1);
				}
				else if (_actualZone == 1) {
					UpdatePatrolPoints(_puerta2_1, _puerta3_0, _point2_3);
				}
				else if (_actualZone == 0) {
					UpdatePatrolPoints(_puerta1_0, _puerta2_0, _point1_2);
				}
			}
			break;
		}
		case 2: {
			//2 posibles caminos dependiendo de las puertas
			/*	Si Puerta1 y Puerta2 abiertas ->*/
			if (_doorStates[0] && _doorStates[1]) {
				if (_actualZone == 0 || _actualZone == 3 || _actualZone == 1) {
					UpdatePatrolPoints(_puerta4_1, _puerta3_0, _puerta1_1);
				}
				else if (_actualZone == 2) {
					UpdatePatrolPoints(_puerta3_1, _puerta4_0, _point3_4);
				}
			}
			/*	Si Puerta1 y Puerta3 abiertas ->*/
			if (_doorStates[0] && _doorStates[2]) {
				if (_actualZone == 0 || _actualZone == 3) {
					UpdatePatrolPoints(_puerta4_1, _puerta2_0, _point3_4) ;//_puerta1_1
				}
				else if (_actualZone == 1 || _actualZone == 2) {
					UpdatePatrolPoints(_puerta2_1, _puerta4_0, _puerta3_0);
				}
			}
			/*Si Puerta1 y Puerta4 abiertas ->*/
			if (_doorStates[0] && _doorStates[3]) {
				if (_actualZone == 0 || _actualZone ==2 || _actualZone == 3) {
					UpdatePatrolPoints(_puerta3_1, _puerta2_0, _point4_1);
				}
				else if (_actualZone == 1) {
					UpdatePatrolPoints(_puerta2_1, _puerta3_0, _point2_3);
				}
			}
			/*Si Puerta2 y Puerta3 abiertas ->*/
			if (_doorStates[1] && _doorStates[2]) {
				if (_actualZone == 0 || _actualZone == 1 || _actualZone == 2) {
					UpdatePatrolPoints(_puerta1_0, _puerta4_0, _point2_3);
				}
				else if (_actualZone == 3) {
					UpdatePatrolPoints(_puerta4_1, _puerta1_1, _point4_1);
				}
			}
			/*Si Puerta2 y Puerta4 abiertas ->*/
			if (_doorStates[1] && _doorStates[3]) {
				if (_actualZone == 0 || _actualZone == 1) {
					UpdatePatrolPoints(_puerta1_0, _puerta3_0, _puerta2_0);
				}
				else if (_actualZone == 3 || _actualZone == 2) {
					UpdatePatrolPoints(_puerta3_1, _puerta1_1, _point4_1);
				}
			}
			/*Si Puerta3 y Puerta4 abiertas ->*/
			if (_doorStates[2] && _doorStates[3]) {
				if (_actualZone == 1 || _actualZone == 2 || _actualZone == 3 ) {
					UpdatePatrolPoints(_puerta1_1, _puerta2_1, _puerta4_0);
				}
				else if (_actualZone == 0) {
					UpdatePatrolPoints(_puerta1_0, _puerta2_0, _point1_2);
				}
			}
			break;
		}
		case 3: {
			//1 posible camino
			/*Si Puerta1 cerrada*/
			if (!_doorStates[0]) {
				UpdatePatrolPoints(_puerta1_0, _puerta1_1, _point3_4);
			}
			else if (!_doorStates[1]) {
				UpdatePatrolPoints(_puerta2_1, _puerta2_0, _point4_1);
			}
			else if (!_doorStates[2]) {
				UpdatePatrolPoints(_puerta3_0, _puerta3_1, _puerta1_1);
			}
			else if (!_doorStates[3]) {
				UpdatePatrolPoints(_puerta4_0, _puerta4_1, _puerta1_1);
			}
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

void AGameStatePlay::UpdateActualZone(AActor* zone) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("GameState: UpdateActualZone! ")));
	TArray<FName> _tags = zone->Tags;

	for (int ind = 0; ind < _tags.Num(); ++ind) {
		if (_tags[ind].ToString() == "zone0") {
			_actualZone = 0;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("GameState: zone 0! ")));
		}
		else if (_tags[ind].ToString() == "zone1") {
			_actualZone = 1;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("GameState: zone 1! ")));
		}
		else if (_tags[ind].ToString() == "zone2") {
			_actualZone = 2;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("GameState: zone 2! ")));
		}
		else if (_tags[ind].ToString() == "zone3") {
			_actualZone = 3;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("GameState: zone 3! ")));
		}
	}
}

void AGameStatePlay::UpdatePatrolPoints(FVector pp1,FVector pp2, FVector pp3) {
	_patrolPoint1->SetActorLocation(pp1);
	_patrolPoint2->SetActorLocation(pp2);
	_patrolPoint3->SetActorLocation(pp3);
}