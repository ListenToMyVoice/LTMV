// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Menu.h"

#include "InputMenu.h"

AMenu::AMenu() {
    PrimaryActorTick.bCanEverTick = true;
    
    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));

    _Slot_NewGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_NewGame"));
    _Slot_NewGameReleasedDelegate.BindUObject(this, &AMenu::NewGame);
    _Slot_NewGame->AddOnInputMenuDelegate(_Slot_NewGameReleasedDelegate, false);
    _Slot_NewGame->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    _Slot_NewGame->RelativeLocation = FVector(0, 0, 0);
    _Slot_NewGame->RelativeRotation = FRotator(0, 0, 0);

    _Text_NewGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_NewGame"));
    _Text_NewGame->SetText(FText::FromString("NEW GAME"));
    _Text_NewGame->AttachToComponent(_Slot_NewGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_ExitGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_ExitGame"));
    _Slot_ExitGameReleasedDelegate.BindUObject(this, &AMenu::ExitGame);
    _Slot_ExitGame->AddOnInputMenuDelegate(_Slot_ExitGameReleasedDelegate, false);
    _Slot_ExitGame->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    _Slot_ExitGame->RelativeLocation = FVector(0, 0, 0);
    _Slot_ExitGame->RelativeRotation = FRotator(0, 0, 0);

    _Text_ExitGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_ExitGame"));
    _Text_ExitGame->SetText(FText::FromString("EXIT GAME"));
    _Text_ExitGame->AttachToComponent(_Slot_ExitGame, FAttachmentTransformRules::KeepRelativeTransform);
}

void AMenu::BeginPlay() {
    Super::BeginPlay();

}

void AMenu::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

/*********************************** BINDINGS ****************************************************/
void AMenu::NewGame() {
    //FGenericPlatformMisc::RequestExit(false);
    ULibraryUtils::Log("NewGame");
}

void AMenu::ExitGame() {
    //FGenericPlatformMisc::RequestExit(false);
    ULibraryUtils::Log("ExitGame");
}