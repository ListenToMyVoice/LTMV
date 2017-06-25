// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Blueprint/UserWidget.h"

#include "NWGameInstance.h"
#include "Tutorial.h"


// Sets default values for this component's properties
UTutorial::UTutorial()
{

	_tutWidgets = {};
	_tutWidgets_EN = {};
	_tutWidgets_FR = {};
	_tutExpected = {};
	_actualWidgets = {};

	PrimaryComponentTick.bCanEverTick = true;

	//WIDGETS en español
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass0(TEXT("/Game/BluePrints/Tutorial/Tutorial0"));
	if (UserInterfaceWidgetClass0.Succeeded()) { _TutorialWidget0 = UserInterfaceWidgetClass0.Class; }
	_tutWidgets.Add(_TutorialWidget0);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass1(TEXT("/Game/BluePrints/Tutorial/Tutorial1"));
	if (UserInterfaceWidgetClass1.Succeeded()) { _TutorialWidget1 = UserInterfaceWidgetClass1.Class; }
	_tutWidgets.Add(_TutorialWidget1);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass2(TEXT("/Game/BluePrints/Tutorial/Tutorial2"));
	if (UserInterfaceWidgetClass2.Succeeded()) { _TutorialWidget2 = UserInterfaceWidgetClass2.Class; }
	_tutWidgets.Add(_TutorialWidget2);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass3(TEXT("/Game/BluePrints/Tutorial/Tutorial3"));
	if (UserInterfaceWidgetClass3.Succeeded()) { _TutorialWidget3 = UserInterfaceWidgetClass3.Class; }
	_tutWidgets.Add(_TutorialWidget3);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass4(TEXT("/Game/BluePrints/Tutorial/Tutorial4"));
	if (UserInterfaceWidgetClass4.Succeeded()) { _TutorialWidget4 = UserInterfaceWidgetClass4.Class; }
	_tutWidgets.Add(_TutorialWidget4);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass5(TEXT("/Game/BluePrints/Tutorial/Tutorial5"));
	if (UserInterfaceWidgetClass5.Succeeded()) { _TutorialWidget5 = UserInterfaceWidgetClass5.Class; }
	_tutWidgets.Add(_TutorialWidget5);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass6(TEXT("/Game/BluePrints/Tutorial/Tutorial6"));
	if (UserInterfaceWidgetClass6.Succeeded()) { _TutorialWidget6 = UserInterfaceWidgetClass6.Class; }
	_tutWidgets.Add(_TutorialWidget6);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass7(TEXT("/Game/BluePrints/Tutorial/Tutorial7"));
	if (UserInterfaceWidgetClass7.Succeeded()) { _TutorialWidget7 = UserInterfaceWidgetClass7.Class; }
	_tutWidgets.Add(_TutorialWidget7);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClassStart(TEXT("/Game/BluePrints/Tutorial/Tutorial0_1"));
	if (UserInterfaceWidgetClassStart.Succeeded()) { _TutorialWidgetStart = UserInterfaceWidgetClassStart.Class; }
	_tutWidgets.Add(_TutorialWidgetStart);

	//WIDGETS en inglés
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass0EN(TEXT("/Game/BluePrints/Tutorial/English/Tutorial0_en"));
	if (UserInterfaceWidgetClass0EN.Succeeded()) { _TutorialWidget0 = UserInterfaceWidgetClass0EN.Class; }
	_tutWidgets_EN.Add(_TutorialWidget0);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass1EN(TEXT("/Game/BluePrints/Tutorial/English/Tutorial1_en"));
	if (UserInterfaceWidgetClass1EN.Succeeded()) { _TutorialWidget1 = UserInterfaceWidgetClass1EN.Class; }
	_tutWidgets_EN.Add(_TutorialWidget1);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass2EN(TEXT("/Game/BluePrints/Tutorial/English/Tutorial2_en"));
	if (UserInterfaceWidgetClass2EN.Succeeded()) { _TutorialWidget2 = UserInterfaceWidgetClass2EN.Class; }
	_tutWidgets_EN.Add(_TutorialWidget2);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass3EN(TEXT("/Game/BluePrints/Tutorial/English/Tutorial3_en"));
	if (UserInterfaceWidgetClass3EN.Succeeded()) { _TutorialWidget3 = UserInterfaceWidgetClass3EN.Class; }
	_tutWidgets_EN.Add(_TutorialWidget3);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass4EN(TEXT("/Game/BluePrints/Tutorial/English/Tutorial4_en"));
	if (UserInterfaceWidgetClass4EN.Succeeded()) { _TutorialWidget4 = UserInterfaceWidgetClass4EN.Class; }
	_tutWidgets_EN.Add(_TutorialWidget4);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass5EN(TEXT("/Game/BluePrints/Tutorial/English/Tutorial5_en"));
	if (UserInterfaceWidgetClass5EN.Succeeded()) { _TutorialWidget5 = UserInterfaceWidgetClass5EN.Class; }
	_tutWidgets_EN.Add(_TutorialWidget5);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass6EN(TEXT("/Game/BluePrints/Tutorial/English/Tutorial6_en"));
	if (UserInterfaceWidgetClass6EN.Succeeded()) { _TutorialWidget6 = UserInterfaceWidgetClass6EN.Class; }
	_tutWidgets_EN.Add(_TutorialWidget6);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass7EN(TEXT("/Game/BluePrints/Tutorial/English/Tutorial7_en"));
	if (UserInterfaceWidgetClass7EN.Succeeded()) { _TutorialWidget7 = UserInterfaceWidgetClass7EN.Class; }
	_tutWidgets_EN.Add(_TutorialWidget7);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClassStartEN(TEXT("/Game/BluePrints/Tutorial/English/Tutorial0_1_en"));
	if (UserInterfaceWidgetClassStartEN.Succeeded()) { _TutorialWidgetStart = UserInterfaceWidgetClassStartEN.Class; }
	_tutWidgets_EN.Add(_TutorialWidgetStart);


	//WIDGETS en frances
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass0FR(TEXT("/Game/BluePrints/Tutorial/French/Tutorial0_fr"));
	if (UserInterfaceWidgetClass0FR.Succeeded()) { _TutorialWidget0 = UserInterfaceWidgetClass0FR.Class; }
	_tutWidgets_FR.Add(_TutorialWidget0);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass1FR(TEXT("/Game/BluePrints/Tutorial/French/Tutorial1_fr"));
	if (UserInterfaceWidgetClass1FR.Succeeded()) { _TutorialWidget1 = UserInterfaceWidgetClass1FR.Class; }
	_tutWidgets_FR.Add(_TutorialWidget1);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass2FR(TEXT("/Game/BluePrints/Tutorial/French/Tutorial2_fr"));
	if (UserInterfaceWidgetClass2FR.Succeeded()) { _TutorialWidget2 = UserInterfaceWidgetClass2FR.Class; }
	_tutWidgets_FR.Add(_TutorialWidget2);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass3FR(TEXT("/Game/BluePrints/Tutorial/French/Tutorial3_fr"));
	if (UserInterfaceWidgetClass3FR.Succeeded()) { _TutorialWidget3 = UserInterfaceWidgetClass3FR.Class; }
	_tutWidgets_FR.Add(_TutorialWidget3);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass4FR(TEXT("/Game/BluePrints/Tutorial/French/Tutorial4_fr"));
	if (UserInterfaceWidgetClass4FR.Succeeded()) { _TutorialWidget4 = UserInterfaceWidgetClass4FR.Class; }
	_tutWidgets_FR.Add(_TutorialWidget4);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass5FR(TEXT("/Game/BluePrints/Tutorial/French/Tutorial5_fr"));
	if (UserInterfaceWidgetClass5FR.Succeeded()) { _TutorialWidget5 = UserInterfaceWidgetClass5FR.Class; }
	_tutWidgets_FR.Add(_TutorialWidget5);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass6FR(TEXT("/Game/BluePrints/Tutorial/French/Tutorial6_fr"));
	if (UserInterfaceWidgetClass6FR.Succeeded()) { _TutorialWidget6 = UserInterfaceWidgetClass6FR.Class; }
	_tutWidgets_FR.Add(_TutorialWidget6);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass7FR(TEXT("/Game/BluePrints/Tutorial/French/Tutorial7_fr"));
	if (UserInterfaceWidgetClass7FR.Succeeded()) { _TutorialWidget7 = UserInterfaceWidgetClass7FR.Class; }
	_tutWidgets_FR.Add(_TutorialWidget7);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClassStartFR(TEXT("/Game/BluePrints/Tutorial/French/Tutorial0_1_fr"));
	if (UserInterfaceWidgetClassStartFR.Succeeded()) { _TutorialWidgetStart = UserInterfaceWidgetClassStartFR.Class; }
	_tutWidgets_FR.Add(_TutorialWidgetStart);

	_tutExpected.Add(false);
	_tutExpected.Add(false);
	_tutExpected.Add(false);
	_tutExpected.Add(false);
	_tutExpected.Add(false);
	_tutExpected.Add(false);
	_tutExpected.Add(false);
	_tutExpected.Add(false);
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

		_TutorialWidget = CreateWidget<UUserWidget>(PlayerController, _actualWidgets[_last]);
		if (_TutorialWidget) {
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

void UTutorial::SetLanguage(FString Language) {

	//Seleccionamos el idioma para los widget dependiendo del lenguaje
	if (Language == "EN")	_actualWidgets = _tutWidgets_EN;
	else if (Language == "ES")	_actualWidgets = _tutWidgets;
	else if (Language == "FR")	_actualWidgets = _tutWidgets_FR;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("El tutorial es en %s"), *Language));
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
			_TutorialWidget = CreateWidget<UUserWidget>(PlayerController, _actualWidgets[index]);

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
