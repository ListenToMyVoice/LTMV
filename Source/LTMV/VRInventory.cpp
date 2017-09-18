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

    CanvasWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InventoryInterface"));
    CanvasWidget->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);  
    CanvasWidget->SetWidgetClass(CanvasWidgetSubclass);
}

// Called when the game starts or when spawned
void AVRInventory::BeginPlay()
{
	Super::BeginPlay();

    SetActorHiddenInGame(bIsVRInventoryHidden);
}

// Called every frame
void AVRInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UWidgetComponent* AVRInventory::InitWidgetComp(UWidgetComponent* WidgetComponent) {
    FVector _Scale = FVector(-8.f, 0.f, 29.f);
    WidgetComponent->SetRelativeLocation(_Scale);
    _Scale = FVector(0.2f, 0.2f, 0.2f);
    WidgetComponent->SetRelativeScale3D(_Scale);
    FRotator _Rotation;
    _Rotation.Yaw = 180.0f;
    _Rotation.Pitch = 0.0f;
    _Rotation.Roll = 0.0f;
    WidgetComponent->AddWorldRotation(_Rotation);

    return WidgetComponent;
}

void AVRInventory::ToggleVRInventory(FVector Location, FRotator Rotation) {
    if (!bIsVRInventoryHidden) {
        SetActorHiddenInGame(!bIsVRInventoryHidden);
    }
    else {
        SetActorHiddenInGame(bIsVRInventoryHidden);
        SetActorLocationAndRotation(Location, Rotation);
    }
    bIsVRInventoryHidden = !bIsVRInventoryHidden;
}