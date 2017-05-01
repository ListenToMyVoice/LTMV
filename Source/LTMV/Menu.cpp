// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Menu.h"

#include "InputMenu.h"

AMenu::AMenu() {
    PrimaryActorTick.bCanEverTick = true;

    _Slot_NewGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_NewGame"));
    _Text_NewGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_NewGame"));
    _Text_NewGame->SetText(FText::FromString("NEW GAME"));
    _Text_NewGame->AttachToComponent(_Slot_NewGame, FAttachmentTransformRules::KeepRelativeTransform);

    //_Slot_Options = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_Options"));
    //_Slot_ExitGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_ExitGame"));

    _Slot_NewGameReleasedDelegate.BindUObject(this, &AMenu::ExitGame);
}

void AMenu::BeginPlay() {
    Super::BeginPlay();

}

void AMenu::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void AMenu::ExitGame() {
    FGenericPlatformMisc::RequestExit(false);
}