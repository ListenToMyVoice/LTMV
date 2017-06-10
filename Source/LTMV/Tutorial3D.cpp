// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Tutorial3D.h"


// Sets default values
ATutorial3D::ATutorial3D()
{
	_tutWidgets = {};
	_tutExpected = {};
	_isTutorialShown = false;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));

	/*** STATIC MESH ***/
	_Decorator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Decorator"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder(
		TEXT("StaticMesh'/Game/Art/Common/Tutorial/fondo.fondo'"));//StaticMesh'/Game/Art/Common/Tutorial/fondo.fondo'
	_Decorator->SetStaticMesh(Finder.Object);
	FVector _Scale = FVector(1.0f, 2.0f, 5.0f);
	_Decorator->SetWorldScale3D(_Scale);
	_Decorator->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	/*** light ***/
	_Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("spotlight"));
	_Light->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_Scale = FVector(-168.0f, 0.0f, 56.0f);
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
	
	/*** widget component attached***/
	_TutComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("widgetComponent"));
	_TutComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_Scale = FVector(-8.f,3.f,19.f);
	_TutComp->SetRelativeLocation(_Scale);
	_Scale = FVector(0.3125f, 0.3f, 0.3125f);
	_TutComp->SetRelativeScale3D(_Scale);
	_Rotation.Yaw = 180.0f;
	_Rotation.Pitch = 0.0f;
	_Rotation.Roll = 0.0f;
	_TutComp->AddWorldRotation(_Rotation);

	//Find UuserWidget blueprints
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClassStart0(TEXT("/Game/BluePrints/Tutorial/Tutorial0"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClassStart0.Succeeded()) { _TutorialWidget0 = UserInterfaceWidgetClassStart0.Class; }
	_tutWidgets.Add(_TutorialWidget0);
	_tutExpected.Add(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClassStart1(TEXT("/Game/BluePrints/Tutorial/Tutorial1"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClassStart1.Succeeded()) { _TutorialWidget1 = UserInterfaceWidgetClassStart1.Class; }
	_tutWidgets.Add(_TutorialWidget1);
	_tutExpected.Add(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClassStart(TEXT("/Game/BluePrints/Tutorial/Tutorial0_1"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClassStart.Succeeded()) { _TutorialWidgetStart = UserInterfaceWidgetClassStart.Class; }
	_tutWidgets.Add(_TutorialWidgetStart);
	_tutExpected.Add(false);


}


void ATutorial3D::ShowTutorial(FVector Location, FRotator Rotation, int index) {
	_isTutorialShown = true;//Tutorial actor showing

	//Widget to show
	_TutComp->SetWidgetClass(_tutWidgets[index]);

	//Enable visibility of actor
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Changed location+rotation of tut actor %s"), *Location.ToString()));

	//Change Rotation and Location of actor
	SetActorLocationAndRotation(Location, Rotation, false, nullptr, ETeleportType::TeleportPhysics);
		
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

