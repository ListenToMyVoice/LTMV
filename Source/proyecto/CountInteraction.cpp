// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "ItemOverlap.h"
#include "PlayerSwitcher.h"
#include "SoundState.h"
#include "DoorState.h"
#include "Engine/StaticMeshActor.h"
#include "CountInteraction.h"

bool _used; 
// Sets default values for this component's properties
UCountInteraction::UCountInteraction()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCountInteraction::BeginPlay()
{

	_used = false;
	Super::BeginPlay();
}


void UCountInteraction::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction) {

	//Si el numero son 2, usar el componente doorstate del muro
	if (_NumInteractions == 2 && !_used) {

		//Mover hacia arriba el componente de doorState de este actor
		UActorComponent* component = this->GetOwner()->GetComponentByClass(UDoorState::StaticClass());
		UDoorState* _doorState = Cast<UDoorState>(component);
		_doorState->SwitchState_Implementation();
		//Cambiar el estado a block para que no se mueva más
		_doorState->_block = true;

		//Hacer sonar el sonido del muro
		UActorComponent* component3 = this->GetOwner()->GetComponentByClass(USoundState::StaticClass());
		USoundState* _sound = Cast<USoundState>(component3);
		_sound->SwitchState_Implementation();

		//Esperar un poco

		//Abrir los componentes DoorState de las puertas
		UActorComponent* component2 = this->GetOwner()->GetComponentByClass(UItemOverlap::StaticClass());
		UPlayerSwitcher*  _playerSwitcher = Cast<UPlayerSwitcher>(component2);
		_playerSwitcher->Use_Implementation();
		/*
		UActorComponent* component2 = this->GetOwner()->GetComponentByClass(UItemOverlap::StaticClass());
		if (component2->GetClass()->ImplementsInterface(UItfSwitcheable::StaticClass())) {
		IItfSwitcheable* itfObject = Cast<IItfSwitcheable>(component2);
		if (itfObject) itfObject->Execute_SwitchState(component2);
		}
		*/
		_used = true;

	}
}

int UCountInteraction::SwitchState2_Implementation() {
	_NumInteractions++;

	return 0;
}

int UCountInteraction::SwitchState_Implementation() {
	//Si se ha llamado con la tecla press sumar 1
	_NumInteractions--;
	
	/*
	FRotator Rotation;
	FVector Position;

	UStaticMeshComponent* meshComp = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(
		UStaticMeshComponent::StaticClass()));


	meshComp->SetMobility(EComponentMobility::Movable);

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
	*/
	return 0;
	
}
