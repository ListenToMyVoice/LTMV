// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Menu.h"

#include "InputMenu.h"

AMenu::AMenu() {
    PrimaryActorTick.bCanEverTick = true;
    
    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));

    /*** MAIN MENU ***/
    _Menu_Main = CreateDefaultSubobject<USceneComponent>(TEXT("_Menu_Main"));
    _Menu_Main->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    _Menu_Main->RelativeLocation = FVector(0, 0, 0);
    _Menu_Main->RelativeRotation = FRotator(0, 0, 0);

    _Slot_NewGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_NewGame"));
    _Slot_NewGameReleasedDelegate.BindUObject(this, &AMenu::BuildMenu_NewGame);
    _Slot_NewGame->AddOnInputMenuDelegate(_Slot_NewGameReleasedDelegate, false);
    _Slot_NewGame->AttachToComponent(_Menu_Main, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_NewGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_NewGame"));
    _Text_NewGame->SetText(FText::FromString("NEW GAME"));
    _Text_NewGame->AttachToComponent(_Slot_NewGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_ExitGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_ExitGame"));
    _Slot_ExitGameReleasedDelegate.BindUObject(this, &AMenu::ExitGame);
    _Slot_ExitGame->AddOnInputMenuDelegate(_Slot_ExitGameReleasedDelegate, false);
    _Slot_ExitGame->AttachToComponent(_Menu_Main, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_ExitGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_ExitGame"));
    _Text_ExitGame->SetText(FText::FromString("EXIT GAME"));
    _Text_ExitGame->AttachToComponent(_Slot_ExitGame, FAttachmentTransformRules::KeepRelativeTransform);

    /*** NEW GAME MENU ***/
    _Menu_NewGame = CreateDefaultSubobject<USceneComponent>(TEXT("_Menu_NewGame"));
    _Menu_NewGame->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    _Menu_NewGame->RelativeLocation = FVector(0, 0, 0);
    _Menu_NewGame->RelativeRotation = FRotator(0, 0, 0);

    _Slot_HostGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_HostGame"));
    _Slot_HostGameReleasedDelegate.BindUObject(this, &AMenu::HostGame);
    _Slot_HostGame->AddOnInputMenuDelegate(_Slot_HostGameReleasedDelegate, false);
    _Slot_HostGame->AttachToComponent(_Menu_NewGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_HostGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_HostGame"));
    _Text_HostGame->SetText(FText::FromString("HOST GAME"));
    _Text_HostGame->AttachToComponent(_Slot_HostGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_FindGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_FindGame"));
    _Slot_FindGameReleasedDelegate.BindUObject(this, &AMenu::FindGame);
    _Slot_FindGame->AddOnInputMenuDelegate(_Slot_FindGameReleasedDelegate, false);
    _Slot_FindGame->AttachToComponent(_Menu_NewGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_FindGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_FindGame"));
    _Text_FindGame->SetText(FText::FromString("FIND GAME"));
    _Text_FindGame->AttachToComponent(_Slot_FindGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_GoBack = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_GoBack"));
    _Slot_GoBackReleasedDelegate.BindUObject(this, &AMenu::BuildMenu_Main);
    _Slot_GoBack->AddOnInputMenuDelegate(_Slot_GoBackReleasedDelegate, false);
    _Slot_GoBack->AttachToComponent(_Menu_NewGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_GoBack = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_GoBack"));
    _Text_GoBack->SetText(FText::FromString("GO BACK"));
    _Text_GoBack->AttachToComponent(_Slot_GoBack, FAttachmentTransformRules::KeepRelativeTransform);
}

void AMenu::BeginPlay() {
    Super::BeginPlay();

    BuildMenu_Main();
}

void AMenu::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

/*********************************** BINDINGS ****************************************************/
void AMenu::BuildMenu_Main() {
    ULibraryUtils::Log("BuildMenu_Main");
    EnableSubmenu(_Menu_NewGame, false);
    EnableSubmenu(_Menu_Main, true);
}

void AMenu::BuildMenu_NewGame() {
    ULibraryUtils::Log("BuildMenu_NewGame");
    EnableSubmenu(_Menu_Main, false);
    EnableSubmenu(_Menu_NewGame, true);
}

void AMenu::ExitGame() {
    ULibraryUtils::Log("ExitGame");
    FGenericPlatformMisc::RequestExit(false);
}

void AMenu::HostGame() {
    ULibraryUtils::Log("HostGame");
}

void AMenu::FindGame() {
    ULibraryUtils::Log("FindGame");
}

/*********************************** MENU TAB ****************************************************/
void AMenu::EnableSubmenu(USceneComponent* Submenu, bool Enable) {
    TArray<USceneComponent*> ChildComponents;
    Submenu->GetChildrenComponents(true, ChildComponents);

    for (USceneComponent* Component : ChildComponents) {
        Component->SetActive(Enable);
        Component->SetHiddenInGame(!Enable, true);
        Component->SetComponentTickEnabled(Enable);
        Component->SetVisibility(Enable, true);
    }
}