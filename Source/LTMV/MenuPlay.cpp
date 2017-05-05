// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "MenuPlay.h"

#include "InputMenu.h"

AMenuPlay::AMenuPlay(const class FObjectInitializer& OI) : Super(OI) {
    /*** PLAY GAME MENU ***/
    _Menu_PlayGame = CreateDefaultSubobject<USceneComponent>(TEXT("_Menu_PlayGame"));
    _Menu_PlayGame->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_BackToMenu = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_BackToMenu"));
    _Slot_BackToMenu->AttachToComponent(_Menu_PlayGame, FAttachmentTransformRules::KeepRelativeTransform);
    _Text_BackToMenu = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_BackToMenu"));
    _Text_BackToMenu->SetText(FText::FromString("BACK TO MENU"));
    _Text_BackToMenu->AttachToComponent(_Slot_BackToMenu, FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_ExitGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_ExitGame2"));
    _Slot_ExitGame->AttachToComponent(_Menu_PlayGame, FAttachmentTransformRules::KeepRelativeTransform);
    _Text_ExitGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_ExitGame"));
    _Text_ExitGame->SetText(FText::FromString("EXIT GAME"));
    _Text_ExitGame->AttachToComponent(_Slot_ExitGame, FAttachmentTransformRules::KeepRelativeTransform);

    BuildLayout();
}

void AMenuPlay::BuildLayout() {
    float VerticalLocation = 0;
    /************************************ PLAY GAME MENU *****************************************/
    _Menu_PlayGame->RelativeLocation = FVector(200, 0, 0);
    _Menu_PlayGame->RelativeRotation = FRotator(0, 180, 0);

    _Slot_BackToMenu->RelativeLocation = FVector(0, 0, VerticalLocation);
    VerticalLocation -= MeshHeight;
    _Text_BackToMenu->RelativeLocation = FVector(MeshDepth, MeshWidth / 2, 0);

    _Slot_ExitGame->RelativeLocation = FVector(0, 0, VerticalLocation);
    _Text_ExitGame->RelativeLocation = FVector(MeshDepth, MeshWidth / 2, 0);
}

void AMenuPlay::BeginPlay() {
    Super::BeginPlay();

    BindDelegates();
}

void AMenuPlay::BindDelegates() {
    /*** PLAY GAME MENU ***/
    _Slot_BackToMenuReleasedDelegate.BindUObject(this, &AMenu::OnBackToMenu);
    _Slot_BackToMenu->AddOnInputMenuDelegate(_Slot_BackToMenuReleasedDelegate, false);

    _Slot_ExitGameReleasedDelegate.BindUObject(this, &AMenu::OnExitGame);
    _Slot_ExitGame->AddOnInputMenuDelegate(_Slot_ExitGameReleasedDelegate, false);
}