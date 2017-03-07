// Fill out your copyright notice in the Description page of Project Settings.
#include "proyecto.h"
#include "DoorState.h"

float _start_displacement;

UDoorState::UDoorState() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UDoorState::BeginPlay() {
    Super::BeginPlay();
}

void UDoorState::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction) {


	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator Rotation;
	FVector Position;

	UStaticMeshComponent* meshComp = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(
		UStaticMeshComponent::StaticClass()));
	meshComp->SetMobility(EComponentMobility::Movable);

	//I opening
	if (StateDoor == EStateDoor::OPENING) {
		if (_start_displacement < _max_displacement) {
			if (DoorType == EDoorType::ROTABLE_DOOR) {

				if (ActOn == EOnAxis::X_AXIS) {
					Rotation.Roll = _displacement;
					Rotation.Pitch = 0.0f;
					Rotation.Yaw = 0.0f;

					meshComp->AddRelativeRotation(Rotation);
				}

				if (ActOn == EOnAxis::Y_AXIS) {
					Rotation.Roll = 0.0f;
					Rotation.Pitch = _displacement;
					Rotation.Yaw = 0.0f;

					meshComp->AddRelativeRotation(Rotation);
				}

				if (ActOn == EOnAxis::Z_AXIS) {
					Rotation.Roll = 0.0f;
					Rotation.Pitch = 0.0f;
					Rotation.Yaw = _displacement;

					meshComp->AddRelativeRotation(Rotation);
				}

			}

			if (DoorType == EDoorType::SLIDABLE_DOOR) {

			if (ActOn == EOnAxis::X_AXIS) {
				Position.X = _displacement;
				Position.Y = 0.0f;
				Position.Z = 0.0f;
				meshComp->AddRelativeLocation(Position);
			}

			if (ActOn == EOnAxis::Y_AXIS) {
				Position.X = 0.0f;
				Position.Y = _displacement;
				Position.Z = 0.0f;
				meshComp->AddRelativeLocation(Position);
			}

			if (ActOn == EOnAxis::Z_AXIS) {
				Position.X = 0.0f;
				Position.Y = 0.0f;
				Position.Z = _displacement;
				meshComp->AddRelativeLocation(Position);
			}
		}

			_start_displacement += _displacement;
			_current_displacement = _start_displacement;
		}
		else {
			StateDoor = EStateDoor::OPEN;
		}

	}
	
	else if (StateDoor == EStateDoor::CLOSING) {

		if (_start_displacement >0) {

			if (DoorType == EDoorType::ROTABLE_DOOR) {

				if (ActOn == EOnAxis::X_AXIS) {
					Rotation.Roll = -_displacement;
					Rotation.Pitch = 0.0f;
					Rotation.Yaw = 0.0f;

					meshComp->AddRelativeRotation(Rotation);
				}

				if (ActOn == EOnAxis::Y_AXIS) {
					Rotation.Roll = 0.0f;
					Rotation.Pitch = -_displacement;
					Rotation.Yaw = 0.0f;

					meshComp->AddRelativeRotation(Rotation);
				}

				if (ActOn == EOnAxis::Z_AXIS) {
					Rotation.Roll = 0.0f;
					Rotation.Pitch = 0.0f;
					Rotation.Yaw = -_displacement;

					meshComp->AddRelativeRotation(Rotation);
				}

			}
			if (DoorType == EDoorType::SLIDABLE_DOOR) {

				if (ActOn == EOnAxis::X_AXIS) {
					Position.X = -_displacement;
					Position.Y = 0.0f;
					Position.Z = 0.0f;
					meshComp->AddRelativeLocation(Position);
				}

				if (ActOn == EOnAxis::Y_AXIS) {
					Position.X = 0.0f;
					Position.Y = -_displacement;
					Position.Z = 0.0f;
					meshComp->AddRelativeLocation(Position);
				}

				if (ActOn == EOnAxis::Z_AXIS) {
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
			StateDoor = EStateDoor::CLOSE;
		}
	}

}

int UDoorState::SwitchState2_Implementation() {
	return 0;
}

int UDoorState::SwitchState_Implementation() {

	//Solo interactuar si la puerta no está bloqueada
	if (!_block) {
		//Si la puerta está cerrada, abrirla
		if (StateDoor == EStateDoor::CLOSE) {
			StateDoor = EStateDoor::OPENING;
			_start_displacement = 0;
		}

		//Si la puerta está abierta, cerrarla
		else if (StateDoor == EStateDoor::OPEN) {
			StateDoor = EStateDoor::CLOSING;
			_start_displacement = _max_displacement;
		}
	}

    return 0;
}

