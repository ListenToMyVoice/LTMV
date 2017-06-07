// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Tutorial3D.h"


// Sets default values
ATutorial3D::ATutorial3D()
{
	_tutWidgets = {};
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

	/*** widget component attached***/
	_TutComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("widgetComponent"));
	_TutComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_Scale = FVector(0.2f,0.2f,0.2f);
	_TutComp->SetWorldScale3D(_Scale);
	_Scale = FVector(-20.0f, 10.0f, 0.0f);
	_TutComp->SetWorldLocation(_Scale);
	FRotator _Rotation;
	_Rotation.Yaw = 180.0f;
	_Rotation.Pitch = 0.0f;
	_Rotation.Roll = 0.0f;
	_TutComp->AddWorldRotation(_Rotation);
	/*
	FVector2D _Position= FVector2D(1000.0f, 600.0f);
	_TutComp->SetDrawSize(_Position);
	*/

	//Find UuserWidget blueprint
	static ConstructorHelpers::FClassFinder<UUserWidget> UserInterfaceWidgetClassStart(TEXT("/Game/BluePrints/Tutorial/Tutorial0_1"));//WidgetBlueprint'/Game/BluePrints/Tutorial/VR/Tutorial0_1.Tutorial0_1'
	if (UserInterfaceWidgetClassStart.Succeeded()) { _TutorialWidgetStart = UserInterfaceWidgetClassStart.Class; }
	_tutWidgets.Add(_TutorialWidgetStart);

	//_TutorialWidget = CreateWidget<UUserWidget>(PlayerController, _TutorialWidgetStart);

	_TutComp->SetWidgetClass(_TutorialWidgetStart);
}
void ATutorial3D::ShowTutorial(FVector Location, FRotator Rotation) {
	//Widget to show

	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Changed location+rotation of tut actor %s"), *Location.ToString()));

	SetActorLocationAndRotation(Location, Rotation, false, nullptr, ETeleportType::TeleportPhysics);
		
}
void ATutorial3D::HideTutorial() {
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);

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

