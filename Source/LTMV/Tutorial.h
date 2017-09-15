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

	class UUserWidget* _ActualWidget;

	void StartTutorial(APlayerController* PlayerController);
	void Next(APlayerController* PlayerController, int index, bool timer);
	void SetLanguage(FString language);
	void Hide();
	void Last(APlayerController* PlayerController, int index, bool timer, AFPCharacter* _player);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	TArray<TSubclassOf<class UUserWidget>> _actualWidgets;
	TArray<TSubclassOf<class UUserWidget>> _tutWidgets;
	TArray<TSubclassOf<class UUserWidget>> _tutWidgets_EN;
	TArray<TSubclassOf<class UUserWidget>> _tutWidgets_FR;
	TArray<bool> _tutExpected;

	float _timer;
	bool _isWidgetTimed;

	class UUserWidget* _TutorialWidget;

	TSubclassOf<class UUserWidget> _TutorialWidgetStart;
	TSubclassOf<class UUserWidget> _TutorialWidget0;
	TSubclassOf<class UUserWidget> _TutorialWidget1;
	TSubclassOf<class UUserWidget> _TutorialWidget2;
	TSubclassOf<class UUserWidget> _TutorialWidget3;
	TSubclassOf<class UUserWidget> _TutorialWidget4;
	TSubclassOf<class UUserWidget> _TutorialWidget5;
	TSubclassOf<class UUserWidget> _TutorialWidget6;
	TSubclassOf<class UUserWidget> _TutorialWidget7;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
