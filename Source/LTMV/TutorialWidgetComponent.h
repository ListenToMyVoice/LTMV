// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetComponent.h"
#include "TutorialWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class LTMV_API UTutorialWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	

	public:
		float _RayParameter;

		UTutorialWidgetComponent();
		/*
		UFUNCTION(BlueprintCallable, Category = "Tutorial Interaction")
		void ChangeTutorial();
		*/
	
	
};
