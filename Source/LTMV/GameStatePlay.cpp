// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "GameStatePlay.h"
#include "DoorState.h"
#include "GameModePlay.h"

AGameStatePlay::AGameStatePlay(const class FObjectInitializer& OI) : Super(OI){
    _StatueWakeUp = false;
    _UniqueNetIdArray = {};

	//level

	//Posiciones de los travel point en las puertas
	_puerta1_0 = FVector(4325.0f, 415.0f, 0.0f);
	_puerta1_1 = FVector(4570.0f, 415.0f, 0.0f);
	_puerta2_0 = FVector(4235.0f, 1070.0f, 0.0f);
	_puerta2_1 = FVector(4235.0f, 1305.0f, 0.0f);
	_puerta3_0 = FVector(4840.0f, 1345.0f, 0.0f);
	_puerta3_1 = FVector(4840.0f, 1070.0f, 0.0f);
	_puerta4_0 = FVector(5220.0f, 920.0f, 0.0f);
	_puerta4_1 = FVector(5225.0f, 665.0f, 0.0f);
	// Posiciones de los travel point intermedios cuando son en medio de zonas
	_point1_2 = FVector(4235.0f, 720.0f, 0.0f);
	_point2_3 = FVector(4550.0f, 1345.0f, 0.0f);
	_point3_4 = FVector(5090.0f, 1010.0f, 0.0f);
	_point4_1 = FVector(5450.0f, 75.0f, 0.0f);

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


	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/BluePrints/Assets/tablilla_laberinto_Blueprint.tablilla_laberinto_Blueprint'"));
	if (ItemBlueprint.Object) {
		TablillaBlueprint = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}
	//Guardamos la tablilla actual
	_tablillaLaberintoActual = nullptr;
}

void AGameStatePlay::updateDoors() {
	//first time here
	if (!_patrolPoint1) {
		getPointsAndEnemy();
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("GameState: UpdateDoors! ")));
	//pick values from doors in the world
	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetName() == "puerta1") {
			updateDoor(ActorItr, 0);
		}
		else if (ActorItr->GetName() == "puerta22") {
			updateDoor(ActorItr, 1);
		}
		else if (ActorItr->GetName() == "puerta3") {
			updateDoor(ActorItr, 2);
		}
		else if (ActorItr->GetName() == "puerta4") {
			updateDoor(ActorItr, 3);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Door states: %d,%d,%d,%d"),_doorStates[0], _doorStates[1], _doorStates[2], _doorStates[3]));
	//counting open doors to set mode
	int _openDoors = 0;
	for (int num = 0;num < 4;num++) {
		if (_doorStates[num]) {
			_openDoors++;
		}
	}
	if (_openDoors == 4) {
		_mode = 1;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("GameState: MODO CIRCULAR ")));
	}
	else {
		_mode = 0;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("GameState: MODO LINEAL ")));
	}
	_actualPath = FindPath();
}

void AGameStatePlay::getPointsAndEnemy() {
	//Coger los PatrolPoint del mapa
	for (TActorIterator<ATargetPoint> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr->GetName() == "PatrolPoint1") {
			_patrolPoint1 = *ActorItr;
		}
		else if (ActorItr->GetName() == "PatrolPoint2") {
			_patrolPoint2 = *ActorItr;
		}
		else if (ActorItr->GetName() == "PatrolPoint3") {
			_patrolPoint3 = *ActorItr;
		}
	}

	//Coger al enemigo
	for (TActorIterator<ACharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr->GetName() == "EnemyCharacterAnd") {
			_enemy = *ActorItr;
		}
	}
	//Coger la tablilla
	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr->GetName() == "tablilla_laberinto_Blueprint") {
			_tablillaLaberintoActual = *ActorItr;
		}
	}
}

void AGameStatePlay::updateDoor(TActorIterator<AStaticMeshActor> _door, int value) {

	UDoorState* Door = Cast<UDoorState>(_door->GetComponentByClass(UDoorState::StaticClass()));
	if (Door) {
		if (Door->GetState() == 1) {
			_doorStates[value] = true;
		}
		else if (Door->GetState() == 0) {
			_doorStates[value] = false;
		}
	}
}

TArray<bool> AGameStatePlay::FindPath() {
	//Calcular caminos dependiendo de la posición de las puertas //Contar puertas abiertas
	int _openDoors = 0;
	for (int num = 0;num < 4;num++) {
		if (_doorStates[num]) {
			_openDoors++;
		}
	}

	switch (_openDoors) {
		case 0: {
			//4 Posible paths //Calcular en qué zona está la IA
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
					UpdatePatrolPoints(_puerta2_0, _puerta4_1, _puerta1_1) ;//_puerta1_1
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
				UpdatePatrolPoints(_puerta4_1, _puerta4_0, _puerta1_0);
			}
			break;
		}
		case 4: {
			//No hay caminos
			UpdatePatrolPoints(_puerta4_1, _puerta2_1, _puerta1_1);
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
/*************************************** LEVEL RESETING *******************************************/
void AGameStatePlay::DeleteAsset(AActor* item) {
	item->Destroy();
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("----- DESTRUIDO ITEM")));
}
void AGameStatePlay::ResetLevel() {

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("LEVEL RESET")));

	//Reseting 4 doors
	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetName() == "puerta1") { ResetDoor(ActorItr); }
		else if (ActorItr->GetName() == "puerta22") { ResetDoor(ActorItr); }
		else if (ActorItr->GetName() == "puerta3") { ResetDoor(ActorItr); }
		else if (ActorItr->GetName() == "puerta4") { ResetDoor(ActorItr); }
	}

	//Positioning the enemy in the first zone
	for (TActorIterator<ACharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr->GetName() == "EnemyCharacterAnd") {
			_enemy = *ActorItr;
			_enemy->SetActorLocation(_point1_2);
		}
	}

	//Reconstruir la tablilla
	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		for (FName tag : ActorItr->Tags) {
			if (tag == "TablillaLaberinto") {
				_tablillaLaberintoActual = *ActorItr;
			}
		}
	}
	if (_tablillaLaberintoActual) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("----- DESTRUIDA TABLILLA")));
		_tablillaLaberintoActual->Destroy();
	}
	FActorSpawnParameters SpawnParams;
	FVector location = FVector(5804.0f, 614.0f, 166.0f);
	//FVector location = FVector(5095.0f, 105.0f, 175.0f);
	FRotator rotation = FRotator(90.0f, 0.0f, 0.0f);
	AActor* Tablilla = GetWorld()->SpawnActor<AActor>(TablillaBlueprint, location, rotation, SpawnParams);
	if (Tablilla) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("++++ SPAWN TABLILLA")));
		_tablillaLaberintoActual = Tablilla;
	}
	//Permitir abrir la reja
	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		if (ActorItr->GetName() == "reja_puerta_Blueprint") {
			UDoorState* Door = nullptr;
			Door = Cast<UDoorState>(ActorItr->GetComponentByClass(UDoorState::StaticClass()));
			if (Door) {
				Door->_block = false;
			}
		}
	}
}

void AGameStatePlay::ResetDoor(TActorIterator<AStaticMeshActor> actor){
	UDoorState* Door = nullptr;
	Door = Cast<UDoorState>(actor->GetComponentByClass(UDoorState::StaticClass()));
	if (Door) {
		if (Door->GetState() == 1) {
			Door->SwitchState();
		}
	}

}