// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "TutorialWidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Tutorial.h"


// Sets default values for this component's properties
UTutorial::UTutorial()
{
	_tutWidgets = {};
	_tutExpected = {};

	PrimaryComponentTick.bCanEverTick = true;
	//Finding assets
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass0(TEXT("/Game/BluePrints/Tutorial/Tutorial0"));
	if (UserInterfaceWidgetClass0.Succeeded()){	_TutorialWidget0 = UserInterfaceWidgetClass0.Class;}
	_tutWidgets.Add(_TutorialWidget0);
	_tutExpected.Add(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass1(TEXT("/Game/BluePrints/Tutorial/Tutorial1"));
	if (UserInterfaceWidgetClass1.Succeeded()) { _TutorialWidget1 = UserInterfaceWidgetClass1.Class; }
	_tutWidgets.Add(_TutorialWidget1);
	_tutExpected.Add(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass2(TEXT("/Game/BluePrints/Tutorial/Tutorial2"));
	if (UserInterfaceWidgetClass2.Succeeded()) { _TutorialWidget2 = UserInterfaceWidgetClass2.Class; }
	_tutWidgets.Add(_TutorialWidget2);
	_tutExpected.Add(false);

	_ActualWidget = nullptr;
}


// Called when the game starts
void UTutorial::BeginPlay()
{
	Super::BeginPlay();
}

void UTutorial::StartTutorial(APlayerController* PlayerController) {
	//Show first tutorial level
	_TutorialWidget = CreateWidget<UUserWidget>(PlayerController, _TutorialWidget0);
	if (_TutorialWidget) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Tutorial Widget creado")));
		_TutorialWidget->AddToViewport(9999); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
		_ActualWidget = _TutorialWidget;
	}
}
void UTutorial::Hide() {
	if (_ActualWidget) {
		_ActualWidget->SetVisibility(ESlateVisibility::Hidden);
		_ActualWidget = nullptr;
	}
}

void UTutorial::Next(APlayerController* PlayerController, int index) {
	//Create Widget only if it was never created before
	if (!_tutExpected[index]) {
		//Hide possible tutorials
		Hide();

		//Create widget from index
		_TutorialWidget = CreateWidget<UUserWidget>(PlayerController, _tutWidgets[index]);

		if (_TutorialWidget) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Tutorial Widget paso a %d"),index));
			_TutorialWidget->AddToViewport(9999); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
			_tutExpected[index] = true;//Update this widget state
			_ActualWidget = _TutorialWidget;//Update actual widget
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("El tutorial %d ya ha pasado!!!"), index));
	}
}


// Called every frame
void UTutorial::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

