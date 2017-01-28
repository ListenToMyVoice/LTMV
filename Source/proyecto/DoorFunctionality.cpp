// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "DoorFunctionality.h"


// Sets default values for this component's properties
UDoorFunctionality::UDoorFunctionality()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UDoorFunctionality::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UDoorFunctionality::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UDoorFunctionality::Use_Implementation() {
    // Open Door
    if (!IsDoorOpen()){
        AActor *Owner = UActorComponent::GetOwner();

        FRotator StartPosition = Owner->GetActorRotation();
        FRotator EndPosition = StartPosition + FRotator(0.0f, 0.0f, 90.0f);

        Owner->SetActorRotation(EndPosition.Quaternion());
        bDoorState = true;
    }

    // Close Door
    else {
        AActor *Owner = UActorComponent::GetOwner();

        FRotator StartPosition = Owner->GetActorRotation();
        FRotator EndPosition = StartPosition + FRotator(0.0f, 0.0f, -90.0f);

        Owner->SetActorRotation(EndPosition.Quaternion());
        bDoorState = false;
    }
}

bool UDoorFunctionality::IsDoorOpen() {
    return bDoorState;
}