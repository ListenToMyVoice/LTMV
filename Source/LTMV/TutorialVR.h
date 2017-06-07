// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "TutorialVR.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LTMV_API UTutorialVR : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTutorialVR();

	class ATutorial3D* _TutActor;
	void StartTutorial(UCameraComponent* PlayerCamera);

	void Next(UCameraComponent* PlayerCamera);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
