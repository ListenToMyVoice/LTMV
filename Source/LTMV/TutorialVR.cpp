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

	SpawnTutorial();

	//Showtutorial first one depending on PlayerCamera
	FVector Location = PlayerCamera->GetComponentLocation() +
		(PlayerCamera->GetForwardVector().GetSafeNormal() * 200) +
		(PlayerCamera->GetRightVector().GetSafeNormal()*-100);

	_TutActor->ShowTutorial(Location,
		PlayerCamera->GetComponentRotation(),2);
}
void UTutorialVR::ToggleTutorial() {
	if (_TutActor) _TutActor->ToggleTutorial();
}
void UTutorialVR::SpawnTutorial() {

	//Create actor if it wasnt created before
	if (!_TutActor) {
		UWorld* World = GetWorld();
		_TutActor = World->SpawnActor<ATutorial3D>();
		////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Tutorial 3d actor spawned!!!")));
	}

}
void UTutorialVR::Next(FVector location, FRotator rotation, int index) {

	SpawnTutorial();

	//Calling to Showtutorial
	_TutActor->ShowTutorial(location, rotation, index);

	////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Tutorial VR pasa a %d"), index));

}

void UTutorialVR::SetLanguage(FString Language) {

	SpawnTutorial();
	_TutActor->SetLanguage(Language);
}

// Called every frame
void UTutorialVR::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

