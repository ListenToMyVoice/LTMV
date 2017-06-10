// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
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

	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass3(TEXT("/Game/BluePrints/Tutorial/Tutorial3"));
	if (UserInterfaceWidgetClass3.Succeeded()) { _TutorialWidget3 = UserInterfaceWidgetClass3.Class; }
	_tutWidgets.Add(_TutorialWidget3);
	_tutExpected.Add(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass4(TEXT("/Game/BluePrints/Tutorial/Tutorial4"));
	if (UserInterfaceWidgetClass4.Succeeded()) { _TutorialWidget4 = UserInterfaceWidgetClass4.Class; }
	_tutWidgets.Add(_TutorialWidget4);
	_tutExpected.Add(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass5(TEXT("/Game/BluePrints/Tutorial/Tutorial5"));
	if (UserInterfaceWidgetClass5.Succeeded()) { _TutorialWidget5 = UserInterfaceWidgetClass5.Class; }
	_tutWidgets.Add(_TutorialWidget5);
	_tutExpected.Add(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass6(TEXT("/Game/BluePrints/Tutorial/Tutorial6"));
	if (UserInterfaceWidgetClass6.Succeeded()) { _TutorialWidget6 = UserInterfaceWidgetClass6.Class; }
	_tutWidgets.Add(_TutorialWidget6);
	_tutExpected.Add(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass7(TEXT("/Game/BluePrints/Tutorial/Tutorial7"));
	if (UserInterfaceWidgetClass7.Succeeded()) { _TutorialWidget7 = UserInterfaceWidgetClass7.Class; }
	_tutWidgets.Add(_TutorialWidget7);
	_tutExpected.Add(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClassStart(TEXT("/Game/BluePrints/Tutorial/Tutorial0_1"));
	if (UserInterfaceWidgetClassStart.Succeeded()) { _TutorialWidgetStart = UserInterfaceWidgetClassStart.Class; }
	_tutWidgets.Add(_TutorialWidgetStart);
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
	//only if it's first time running tutorial
	int _last = _tutExpected.Num()-1;
	if (!_tutExpected[_last]) {
		_TutorialWidget = CreateWidget<UUserWidget>(PlayerController, _TutorialWidgetStart);
		if (_TutorialWidget) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Empiueza el tutorial ")));
			_TutorialWidget->AddToViewport(9999); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
			_ActualWidget = _TutorialWidget;//Update actual widget
			_tutExpected[_last] = true;//update this widget as shown

			//Starting timer
			_timer = 0.0f;
			_isWidgetTimed = true;
		}
	}
}


void UTutorial::Hide() {
	if (_ActualWidget) {
		_ActualWidget->SetVisibility(ESlateVisibility::Hidden);
		_ActualWidget = nullptr;
	}
}

void UTutorial::Next(APlayerController* PlayerController, int index, bool timer, bool last) {

	bool _isMomentForLast = true;
	//If its the last tutorial, make sure every other tutorial is in true to create it
	if (last)
	{
		for (int i = 0; i < _tutExpected.Num()-2; i++) {
			if (!_tutExpected[i]) {
				_isMomentForLast = false;
			}
		}
	}

	//Create Widget only if it was never created before
	if (!_tutExpected[index]) {
		if (!last || (last && _isMomentForLast)) {
			//Hide possible tutorials
			Hide();//Hide any possible tutorial widgets active

			//Create widget from index
			_TutorialWidget = CreateWidget<UUserWidget>(PlayerController, _tutWidgets[index]);

			if (_TutorialWidget) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Tutorial Widget paso a %d"), index));
				_TutorialWidget->AddToViewport(9999); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
				_tutExpected[index] = true;//Update this widget state
				_ActualWidget = _TutorialWidget;//Update actual widget
				if (timer) {
					_timer = 0.0f;
					_isWidgetTimed = true;
				}
			}
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

	//If there is a timer put on widget
	if (_isWidgetTimed) {
		if (_timer <= 320.0f) {
			_timer += 1.0f;
		}
		else {
			Hide();
			_isWidgetTimed = false;
			_timer = 0.0f;
		}
	}

}

