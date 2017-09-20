// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "VRInventory.h"

// Sets default values
AVRInventory::AVRInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bIsVRInventoryHidden = true;

    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));

    static ConstructorHelpers::FObjectFinder<UMaterial> CanvasMaterial(TEXT("/Game/LoadScreen/Widget3DPassThrough_Emissive.Widget3DPassThrough_Emissive"));
    if (CanvasMaterial.Succeeded()) MaterialWidget = CanvasMaterial.Object;

    CanvasWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InventoryInterface"));
    CanvasWidget->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    static ConstructorHelpers::FClassFinder<UUserWidget> CanvasInventory(TEXT("/Game/BluePrints/HUD/InventoryHUD_VR.InventoryHUD_VR"));
    if (CanvasInventory.Succeeded()) CanvasWidget->SetWidgetClass(CanvasInventory.Class);
    CanvasWidget->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
    CanvasWidget->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
    CanvasWidget->SetDrawSize(FVector2D(1000.f, 1000.f));
    CanvasWidget->SetBlendMode(EWidgetBlendMode::Masked);
    CanvasWidget->SetMaterial(0, MaterialWidget);
    CanvasWidget->SetTwoSided(true);
}

// Called when the game starts or when spawned
void AVRInventory::BeginPlay()
{
	Super::BeginPlay();

    CanvasWidget->InitWidget();
    SetActorHiddenInGame(bIsVRInventoryHidden);
}

// Called every frame
void AVRInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVRInventory::ToggleVRInventory(FVector Location, FRotator Rotation) {
    if (!bIsVRInventoryHidden) {
        SetActorHiddenInGame(bIsVRInventoryHidden);
    }
    else {
        SetActorHiddenInGame(!bIsVRInventoryHidden);
        SetActorLocationAndRotation(Location, Rotation);
    }
    bIsVRInventoryHidden = !bIsVRInventoryHidden;
}