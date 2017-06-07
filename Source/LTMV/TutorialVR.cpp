// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "TutorialVR.h"


// Sets default values for this component's properties
UTutorialVR::UTutorialVR()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	/*
	*/
}


// Called when the game starts
void UTutorialVR::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTutorialVR::StartTutorial(UCameraComponent* PlayerCamera) {
	if (!_TutActor) {
		UWorld* World = GetWorld();
		_TutActor = World->SpawnActor<ATutorial3D>();//Spawn actor in world if it wasnt spawned yet
		//_TutActor->HideTutorial();//hide the actor

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Tutorial 3d actor spawned!!!")));
	}

	//Showtutorial
	FVector Location = PlayerCamera->GetComponentLocation() +
		(PlayerCamera->GetForwardVector().GetSafeNormal() * 200);

	_TutActor->ShowTutorial(Location,
		PlayerCamera->GetComponentRotation());
}

void UTutorialVR::Next(UCameraComponent* PlayerCamera) {

	if (!_TutActor) {
		UWorld* World = GetWorld();
		_TutActor = World->SpawnActor<ATutorial3D>();//Spawn actor in world if it wasnt spawned yet
		//_TutActor->HideTutorial();//hide the actor

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Tutorial 3d actor spawned!!!")));
	}
	//Showtutorial
	FVector Location = PlayerCamera->GetComponentLocation() +
		(PlayerCamera->GetForwardVector().GetSafeNormal() * 200);

	_TutActor->ShowTutorial(Location,
		PlayerCamera->GetComponentRotation());

}
// Called every frame
void UTutorialVR::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

