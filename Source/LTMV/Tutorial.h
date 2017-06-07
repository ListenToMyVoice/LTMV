// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Tutorial.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LTMV_API UTutorial : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTutorial();

	TSubclassOf<class UUserWidget> _TutorialWidgetStart;
	TSubclassOf<class UUserWidget> _TutorialWidget0;
	TSubclassOf<class UUserWidget> _TutorialWidget1;
	TSubclassOf<class UUserWidget> _TutorialWidget2;
	TSubclassOf<class UUserWidget> _TutorialWidget3;
	TSubclassOf<class UUserWidget> _TutorialWidget4;
	TSubclassOf<class UUserWidget> _TutorialWidget5;
	TSubclassOf<class UUserWidget> _TutorialWidget6;
	TSubclassOf<class UUserWidget> _TutorialWidget7;

	class UUserWidget* _TutorialWidget;
	class UUserWidget* _ActualWidget;

	void StartTutorial(APlayerController* PlayerController);
	void Next(APlayerController* PlayerController, int index, bool timer, bool last);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Hide();

	TArray<TSubclassOf<class UUserWidget>> _tutWidgets;
	TArray<bool> _tutExpected;

	float _timer;
	bool _isWidgetTimed;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
