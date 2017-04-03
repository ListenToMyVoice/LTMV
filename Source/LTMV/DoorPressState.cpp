// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "DoorPressState.h"


// Sets default values for this component's properties
UDoorPressState::UDoorPressState()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDoorPressState::BeginPlay()
{
	Super::BeginPlay();
	_start_displacement = 0;
	
}


// Called every frame
void UDoorPressState::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FRotator Rotation;
	FVector Position;

	UStaticMeshComponent* meshComp = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(
		UStaticMeshComponent::StaticClass()));
	meshComp->SetMobility(EComponentMobility::Movable);

	//I opening
	if (StateDoor == EStateDoor2::OPENING) {
		//Seguir abriendo mientras no lleguemos a max_displacement
		if (_start_displacement < _max_displacement) {
			if (DoorType == EDoorType2::ROTABLE_DOOR) {

				if (ActOn == EOnAxis2::X_AXIS) {
					Rotation.Roll = _displacement;
					Rotation.Pitch = 0.0f;
					Rotation.Yaw = 0.0f;

					meshComp->AddRelativeRotation(Rotation);
				}

				if (ActOn == EOnAxis2::Y_AXIS) {
					Rotation.Roll = 0.0f;
					Rotation.Pitch = _displacement;
					Rotation.Yaw = 0.0f;

					meshComp->AddRelativeRotation(Rotation);
				}

				if (ActOn == EOnAxis2::Z_AXIS) {
					Rotation.Roll = 0.0f;
					Rotation.Pitch = 0.0f;
					Rotation.Yaw = _displacement;

					meshComp->AddRelativeRotation(Rotation);
				}

			}

			if (DoorType == EDoorType2::SLIDABLE_DOOR) {

				if (ActOn == EOnAxis2::X_AXIS) {
					Position.X = _displacement;
					Position.Y = 0.0f;
					Position.Z = 0.0f;
					meshComp->AddRelativeLocation(Position);
				}

				if (ActOn == EOnAxis2::Y_AXIS) {
					Position.X = 0.0f;
					Position.Y = _displacement;
					Position.Z = 0.0f;
					meshComp->AddRelativeLocation(Position);
				}

				if (ActOn == EOnAxis2::Z_AXIS) {
					Position.X = 0.0f;
					Position.Y = 0.0f;
					Position.Z = _displacement;
					meshComp->AddRelativeLocation(Position);
				}
			}

			//Actualizar el estado próximo de _current_displacement
			_start_displacement += _displacement;
			_current_displacement = _start_displacement;
		}
		else {
			StateDoor = EStateDoor2::OPEN;
		}

	}

	else if (StateDoor == EStateDoor2::CLOSING) {

		if (_start_displacement >0) {

			if (DoorType == EDoorType2::ROTABLE_DOOR) {

				if (ActOn == EOnAxis2::X_AXIS) {
					Rotation.Roll = -_displacement;
					Rotation.Pitch = 0.0f;
					Rotation.Yaw = 0.0f;

					meshComp->AddRelativeRotation(Rotation);
				}

				if (ActOn == EOnAxis2::Y_AXIS) {
					Rotation.Roll = 0.0f;
					Rotation.Pitch = -_displacement;
					Rotation.Yaw = 0.0f;

					meshComp->AddRelativeRotation(Rotation);
				}

				if (ActOn == EOnAxis2::Z_AXIS) {
					Rotation.Roll = 0.0f;
					Rotation.Pitch = 0.0f;
					Rotation.Yaw = -_displacement;

					meshComp->AddRelativeRotation(Rotation);
				}

			}

			if (DoorType == EDoorType2::SLIDABLE_DOOR) {

				if (ActOn == EOnAxis2::X_AXIS) {
					Position.X = -_displacement;
					Position.Y = 0.0f;
					Position.Z = 0.0f;
					meshComp->AddRelativeLocation(Position);
				}

				if (ActOn == EOnAxis2::Y_AXIS) {
					Position.X = 0.0f;
					Position.Y = -_displacement;
					Position.Z = 0.0f;
					meshComp->AddRelativeLocation(Position);
				}

				if (ActOn == EOnAxis2::Z_AXIS) {
					Position.X = 0.0f;
					Position.Y = 0.0f;
					Position.Z = -_displacement;
					meshComp->AddRelativeLocation(Position);
				}
			}

			_start_displacement -= _displacement;
			_current_displacement = _start_displacement;
		}
		else {
			StateDoor = EStateDoor2::CLOSE;
		}
	}
}


int UDoorPressState::SwitchState2_Implementation() {
	//Solo interactuar si la puerta no está bloqueada
	if (!_block) {
		//Poner la puerta a opening
		StateDoor = EStateDoor2::OPENING;
		_start_displacement = _current_displacement;
	}
	return 0;
}

int UDoorPressState::SwitchState_Implementation() {

	//Solo interactuar si la puerta no está bloqueada
	if (!_block) {
		//Poner la puerta a closing
		StateDoor = EStateDoor2::CLOSING;
		_start_displacement = _current_displacement;
	}

	return 0;
}