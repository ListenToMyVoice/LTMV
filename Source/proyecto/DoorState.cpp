// Fill out your copyright notice in the Description page of Project Settings.
#include "Lock.h"

#include "proyecto.h"
#include "DoorState.h"


UDoorState::UDoorState() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UDoorState::BeginPlay() {
    Super::BeginPlay();
}

int UDoorState::SwitchState_Implementation() {

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

    return 0;
}

