// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "RotateState.h"


// Sets default values for this component's properties
URotateState::URotateState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URotateState::BeginPlay()
{
	Super::BeginPlay();

    _meshComp = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
    ULibraryUtils::TestNull(_meshComp);
    _initialRotation = _meshComp->GetComponentRotation();

    SetComponentTickEnabled(true);
	// ...
	
}


// Called every frame
void URotateState::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
    if (!_opened) {
        AddRotation();
    }
	// ...
}

int URotateState::SwitchState_Implementation() {

    if (_meshComp->GetComponentRotation().Yaw > _initialRotation.Yaw &&
        _meshComp->GetComponentRotation().Yaw < _displacement) {
        _requestOff = true;
    }
    AddRotation();
        return 0;
}

void URotateState::AddRotation() {
    FRotator Rotation;

    UStaticMeshComponent* meshComp = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(
        UStaticMeshComponent::StaticClass()));

    meshComp->SetMobility(EComponentMobility::Movable);


    if (RotateOnAxis == EMyAxis::X_AXIS) {
        Rotation.Roll = _displacement;
        Rotation.Pitch = 0.0f;
        Rotation.Yaw = 0.0f;

        meshComp->AddRelativeRotation(Rotation);
    }

    if (RotateOnAxis == EMyAxis::Y_AXIS) {
        Rotation.Roll = 0.0f;
        Rotation.Pitch = _displacement;
        Rotation.Yaw = 0.0f;

        meshComp->AddRelativeRotation(Rotation);
    }

    if (RotateOnAxis == EMyAxis::Z_AXIS) {
        Rotation.Roll = 0.0f;
        Rotation.Pitch = 0.0f;
        Rotation.Yaw = _displacement;

        meshComp->AddRelativeRotation(Rotation);
    }
}

