// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "TutorialVR.h"


// Sets default values for this component's properties
UTutorialVR::UTutorialVR()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTutorialVR::BeginPlay()
{
	Super::BeginPlay();
}

void UTutorialVR::StartTutorial(UCameraComponent* PlayerCamera) {

	//Create actor if it wasnt created before
	if (!_TutActor) {
		UWorld* World = GetWorld();
		_TutActor = World->SpawnActor<ATutorial3D>();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Tutorial 3d actor spawned!!!")));
	}

	//Showtutorial first one depending on PlayerCamera
	FVector Location = PlayerCamera->GetComponentLocation() +
		(PlayerCamera->GetForwardVector().GetSafeNormal() * 200);
	_TutActor->ShowTutorial(Location,
		PlayerCamera->GetComponentRotation(),2);
}


void UTutorialVR::Next(FVector location, FRotator rotation, int index) {

	if (!_TutActor) {
		UWorld* World = GetWorld();
		_TutActor = World->SpawnActor<ATutorial3D>();//Spawn actor in world if it wasnt spawned yet
		//_TutActor->HideTutorial();//hide the actor

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Tutorial 3d actor spawned!!!")));
	}

	//Calling to Showtutorial
	_TutActor->ShowTutorial(location, rotation, index);

}
// Called every frame
void UTutorialVR::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

