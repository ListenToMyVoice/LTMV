// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Tutorial3D.h"


// Sets default values
ATutorial3D::ATutorial3D()
{
	_tutExpected = {};
	_tutWidComps = {};
	_tutWidComps_EN = {};
	_tutWidComps_FR = {};
	_actualWidComp = {};

	_isTutorialShown = false;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));

	/*** STATIC MESH ***/
	_Decorator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Decorator"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder(
		TEXT("StaticMesh'/Game/Art/Common/Tutorial/fondo.fondo'"));//StaticMesh'/Game/Art/Common/Tutorial/fondo.fondo'
	_Decorator->SetStaticMesh(Finder.Object);
	FVector _Scale = FVector(1.0f, 1.0f, -7.0f);
	_Decorator->SetWorldScale3D(_Scale);
	_Scale = FVector(.0f, .0f, 100.0f);
	_Decorator->SetRelativeLocation(_Scale);
	_Decorator->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	/*** light ***/
	_Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("spotlight"));
	_Light->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_Scale = FVector(-168.0f, 0.0f, 36.0f);
	_Light->SetRelativeLocation(_Scale);
	FRotator _Rotation = FRotator(0.f, 0.f, 0.f);
	_Light->SetRelativeRotation(_Rotation);
	FLinearColor Color = FLinearColor();
	Color.R = 1.f;
	Color.G = 0.776;
	Color.B = 0.376;
	_Light->SetLightColor(Color, true);
	_Light->SetIntensity(500.f);
	_Light->OuterConeAngle = 32.f;
	_Light->AttenuationRadius = 225.f;
	
	/*************** widget component attached ESPAÑOL*****************/
	_TutComp0 = CreateDefaultSubobject<UWidgetComponent>(TEXT("_TutComp0"));
	_TutComp0->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_TutComp0 = InitWidgetComp(_TutComp0);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass0(TEXT("/Game/BluePrints/Tutorial/Tutorial0"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClass0.Succeeded()) { _TutorialWidget = UserInterfaceWidgetClass0.Class; }
	_TutComp0->SetWidgetClass(_TutorialWidget);
	_tutWidComps.Add(_TutComp0);

	_TutComp1 = CreateDefaultSubobject<UWidgetComponent>(TEXT("_TutComp1"));
	_TutComp1->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_TutComp1 = InitWidgetComp(_TutComp1);//"/Game/BluePrints/Tutorial/Tutorial1"
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass1(TEXT("/Game/BluePrints/Tutorial/Tutorial1"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClass1.Succeeded()) { _TutorialWidget = UserInterfaceWidgetClass1.Class; }
	_TutComp1->SetWidgetClass(_TutorialWidget);
	_tutWidComps.Add(_TutComp1);

	_TutCompStart = CreateDefaultSubobject<UWidgetComponent>(TEXT("_TutCompStart"));
	_TutCompStart->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_TutCompStart = InitWidgetComp(_TutCompStart);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClassStart(TEXT("/Game/BluePrints/Tutorial/VR/Tutorial0_1"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClassStart.Succeeded()) { _TutorialWidget = UserInterfaceWidgetClassStart.Class; }
	_TutCompStart->SetWidgetClass(_TutorialWidget);
	_tutWidComps.Add(_TutCompStart);


	/************** widget component attached INGLES**************/
	_TutComp0_EN = CreateDefaultSubobject<UWidgetComponent>(TEXT("_TutComp0_EN"));
	_TutComp0_EN->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_TutComp0_EN = InitWidgetComp(_TutComp0_EN);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass0EN(TEXT("/Game/BluePrints/Tutorial/Tutorial0"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClass0EN.Succeeded()) { _TutorialWidget = UserInterfaceWidgetClass0EN.Class; }
	_TutComp0_EN->SetWidgetClass(_TutorialWidget);
	_tutWidComps_EN.Add(_TutComp0_EN);

	_TutComp1_EN = CreateDefaultSubobject<UWidgetComponent>(TEXT("_TutComp1_EN"));
	_TutComp1_EN->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_TutComp1_EN = InitWidgetComp(_TutComp1_EN);//"/Game/BluePrints/Tutorial/Tutorial1"
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass1EN(TEXT("/Game/BluePrints/Tutorial/Tutorial1"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClass1EN.Succeeded()) { _TutorialWidget = UserInterfaceWidgetClass1EN.Class; }
	_TutComp1_EN->SetWidgetClass(_TutorialWidget);
	_tutWidComps_EN.Add(_TutComp1_EN);

	_TutCompStart_EN = CreateDefaultSubobject<UWidgetComponent>(TEXT("_TutCompStart_EN"));
	_TutCompStart_EN->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_TutCompStart_EN = InitWidgetComp(_TutCompStart_EN);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClassStartEN(TEXT("/Game/BluePrints/Tutorial/VR/English/Tutorial0_1_en"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClassStartEN.Succeeded()) { _TutorialWidget = UserInterfaceWidgetClassStartEN.Class; }
	_TutCompStart_EN->SetWidgetClass(_TutorialWidget);
	_tutWidComps_EN.Add(_TutCompStart_EN);

	/************** widget component attached FRANCES**************/
	_TutComp0_FR = CreateDefaultSubobject<UWidgetComponent>(TEXT("_TutComp0_FR"));
	_TutComp0_FR->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_TutComp0_FR = InitWidgetComp(_TutComp0_FR);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass0FR(TEXT("/Game/BluePrints/Tutorial/Tutorial0"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClass0FR.Succeeded()) { _TutorialWidget = UserInterfaceWidgetClass0FR.Class; }
	_TutComp0_FR->SetWidgetClass(_TutorialWidget);
	_tutWidComps_FR.Add(_TutComp0_FR);

	_TutComp1_FR = CreateDefaultSubobject<UWidgetComponent>(TEXT("_TutComp1_FR"));
	_TutComp1_FR->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_TutComp1_FR = InitWidgetComp(_TutComp1_FR);//"/Game/BluePrints/Tutorial/Tutorial1"
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClass1FR(TEXT("/Game/BluePrints/Tutorial/Tutorial1"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClass1FR.Succeeded()) { _TutorialWidget = UserInterfaceWidgetClass1FR.Class; }
	_TutComp1_FR->SetWidgetClass(_TutorialWidget);
	_tutWidComps_FR.Add(_TutComp1_FR);

	_TutCompStart_FR = CreateDefaultSubobject<UWidgetComponent>(TEXT("_TutCompStart_FR"));
	_TutCompStart_FR->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_TutCompStart_FR = InitWidgetComp(_TutCompStart_FR);
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClassStartFR(TEXT("/Game/BluePrints/Tutorial/VR/French/Tutorial0_1_fr"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClassStartFR.Succeeded()) { _TutorialWidget = UserInterfaceWidgetClassStartFR.Class; }
	_TutCompStart_FR->SetWidgetClass(_TutorialWidget);
	_tutWidComps_FR.Add(_TutCompStart_FR);


	_tutExpected.Add(false);
	_tutExpected.Add(false);
	_tutExpected.Add(false);
	//Al widget components hidden
	HideWidgets();
}

UWidgetComponent* ATutorial3D::InitWidgetComp(UWidgetComponent* TutComp) {
	FVector _Scale = FVector(-8.f, 0.f, 29.f);
	TutComp->SetRelativeLocation(_Scale);
	_Scale = FVector(0.2f, 0.2f, 0.2f);
	TutComp->SetRelativeScale3D(_Scale);
	FRotator _Rotation;
	_Rotation.Yaw = 180.0f;
	_Rotation.Pitch = 0.0f;
	_Rotation.Roll = 0.0f;
	TutComp->AddWorldRotation(_Rotation);

	return TutComp;
}

void ATutorial3D::ShowTutorial(FVector Location, FRotator Rotation, int index) {
	_isTutorialShown = true;//Tutorial actor showing

	//hide widgets
	HideWidgets();
	//Widget to show
	_actualWidComp[index]->SetVisibility(true);

	//Enable visibility of actor
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Changed location+rotation of tut actor %s"), *Location.ToString()));

	//Change Rotation and Location of actor
	SetActorLocationAndRotation(Location, Rotation, false, nullptr, ETeleportType::TeleportPhysics);
		
}

void ATutorial3D::HideWidgets() {
	for (int i = 0; i < _tutWidComps.Num(); i++) {
		_tutWidComps[i]->SetVisibility(false);
	}
	for (int i = 0; i < _tutWidComps_EN.Num(); i++) {
		_tutWidComps_EN[i]->SetVisibility(false);
	}
	for (int i = 0; i < _tutWidComps_FR.Num(); i++) {
		_tutWidComps_FR[i]->SetVisibility(false);
	}
}
void ATutorial3D::ToggleTutorial() {
	if (_isTutorialShown) {
		SetActorHiddenInGame(false);
		SetActorTickEnabled(true);
	}
	else {
		SetActorHiddenInGame(true);
		SetActorTickEnabled(false);
	}
	_isTutorialShown = !_isTutorialShown;
}

void ATutorial3D::SetLanguage(FString Language) {
	//Seleccionamos el idioma para los widget dependiendo del lenguaje
	if (Language == "EN")	_actualWidComp = _tutWidComps_EN;
	else if (Language == "ES")	_actualWidComp = _tutWidComps;
	else if (Language == "FR")	_actualWidComp = _tutWidComps_FR;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("El tutorial es en %s"), *Language));
}

// Called when the game starts or when spawned
void ATutorial3D::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATutorial3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

