// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "MenuLobby.h"

#include "InputMenu.h"

AMenuLobby::AMenuLobby(const class FObjectInitializer& OI) : Super(OI) {
    /*** LOBBY MENU ***/
    _Menu_Lobby = CreateDefaultSubobject<USceneComponent>(TEXT("_Menu_Lobby"));
    _Menu_Lobby->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_BackToMenu = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_BackToMenu"));
    _Slot_BackToMenu->AttachToComponent(_Menu_Lobby, FAttachmentTransformRules::KeepRelativeTransform);
    _Text_BackToMenu = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_BackToMenu"));
    _Text_BackToMenu->SetText(FText::FromString("BACK TO MENU"));
    _Text_BackToMenu->AttachToComponent(_Slot_BackToMenu, FAttachmentTransformRules::KeepRelativeTransform, FName("Socket"));

    _Slot_ExitGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_ExitGame2"));
    _Slot_ExitGame->AttachToComponent(_Menu_Lobby, FAttachmentTransformRules::KeepRelativeTransform);
    _Text_ExitGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_ExitGame"));
    _Text_ExitGame->SetText(FText::FromString("EXIT GAME"));
    _Text_ExitGame->AttachToComponent(_Slot_ExitGame, FAttachmentTransformRules::KeepRelativeTransform, FName("Socket"));
    
    BuildLayout();
}

void AMenuLobby::BuildLayout() {
    float VerticalLocation = 0;
    /************************************ LOBBY MENU *********************************************/
    _Menu_Lobby->RelativeLocation = _SubmenuLocation;
    _Menu_Lobby->RelativeRotation = _SubmenuRotator;

    _Slot_BackToMenu->RelativeLocation = FVector(0, 0, VerticalLocation);
    VerticalLocation -= MeshHeight;
    _Slot_ExitGame->RelativeLocation = FVector(0, 0, VerticalLocation);
}

void AMenuLobby::BeginPlay() {
    Super::BeginPlay();

    BindDelegates();
}

void AMenuLobby::BindDelegates() {
    /*** LOBBY MENU ***/
    _Slot_BackToMenuReleasedDelegate.BindUObject(this, &AMenu::OnBackToMenu);
    _Slot_BackToMenu->AddOnInputMenuDelegate(_Slot_BackToMenuReleasedDelegate, false);

    _Slot_ExitGameReleasedDelegate.BindUObject(this, &AMenu::OnExitGame);
    _Slot_ExitGame->AddOnInputMenuDelegate(_Slot_ExitGameReleasedDelegate, false);
}