// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "ItemOverlap.h"
#include "PlayerSwitcher.h"
#include "SoundState.h"
#include "DoorState.h"
#include "Engine/StaticMeshActor.h"
#include "CountInteractionState.h"


// Sets default values for this component's properties
UCountInteractionState::UCountInteractionState()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCountInteractionState::BeginPlay()
{
	_used = false;
	Super::BeginPlay();
}


// Called every frame
void UCountInteractionState::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

}


int UCountInteractionState::SwitchState2_Implementation() {
	_NumInteractions++;

	//Si el numero son 2 y aún no se ha usado este componente
	if (_NumInteractions == _max_NumInteractions && !_used) {
		//usar todos los componentes del PlayerSwitcher de este componente
		UActorComponent* component2 = this->GetOwner()->GetComponentByClass(UItemOverlap::StaticClass());
		UPlayerSwitcher*  _playerSwitcher = Cast<UPlayerSwitcher>(component2);
		_playerSwitcher->Press_Implementation();
		_playerSwitcher->Use_Implementation();

		_used = true;
	}

	return 0;
}

int UCountInteractionState::SwitchState_Implementation() {
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
