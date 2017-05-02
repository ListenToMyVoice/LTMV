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
    _Slot_NewGame->AttachToComponent(_Menu_Main, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_NewGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_NewGame"));
    _Text_NewGame->SetText(FText::FromString("NEW GAME"));
    _Text_NewGame->AttachToComponent(_Slot_NewGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_ExitGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_ExitGame"));
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
    _Slot_HostGame->AttachToComponent(_Menu_NewGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_HostGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_HostGame"));
    _Text_HostGame->SetText(FText::FromString("HOST GAME"));
    _Text_HostGame->AttachToComponent(_Slot_HostGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_FindGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_FindGame"));
    _Slot_FindGame->AttachToComponent(_Menu_NewGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_FindGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_FindGame"));
    _Text_FindGame->SetText(FText::FromString("FIND GAME"));
    _Text_FindGame->AttachToComponent(_Slot_FindGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_GoBack = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_GoBack"));
    _Slot_GoBack->AttachToComponent(_Menu_NewGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_GoBack = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_GoBack"));
    _Text_GoBack->SetText(FText::FromString("GO BACK"));
    _Text_GoBack->AttachToComponent(_Slot_GoBack, FAttachmentTransformRules::KeepRelativeTransform);

    /*** FIND GAME MENU ***/
    _Menu_FindGame = CreateDefaultSubobject<USceneComponent>(TEXT("_Menu_FindGame"));
    _Menu_FindGame->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    _Menu_FindGame->RelativeLocation = FVector(0, 0, 0);
    _Menu_FindGame->RelativeRotation = FRotator(0, 0, 0);

    _Slot_JoinGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_JoinGame"));
    _Slot_JoinGame->AttachToComponent(_Menu_FindGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_JoinGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_JoinGame"));
    _Text_JoinGame->SetText(FText::FromString("JOIN GAME"));
    _Text_JoinGame->AttachToComponent(_Slot_JoinGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_GoBack2 = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_GoBack2"));
    _Slot_GoBack2->AttachToComponent(_Menu_FindGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_GoBack2 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_GoBack2"));
    _Text_GoBack2->SetText(FText::FromString("GO BACK"));
    _Text_GoBack2->AttachToComponent(_Slot_GoBack2, FAttachmentTransformRules::KeepRelativeTransform);

    /*** PLAY GAME MENU ***/
    _Menu_PlayGame = CreateDefaultSubobject<USceneComponent>(TEXT("_Menu_PlayGame"));
    _Menu_PlayGame->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    _Menu_PlayGame->RelativeLocation = FVector(0, 0, 0);
    _Menu_PlayGame->RelativeRotation = FRotator(0, 0, 0);

    _Slot_BackToMenu = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_BackToMenu"));
    _Slot_BackToMenu->AttachToComponent(_Menu_PlayGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_BackToMenu = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_BackToMenu"));
    _Text_BackToMenu->SetText(FText::FromString("BACK TO MENU"));
    _Text_BackToMenu->AttachToComponent(_Slot_BackToMenu, FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_ExitGame2 = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_ExitGame2"));
    _Slot_ExitGame2->AttachToComponent(_Menu_PlayGame, FAttachmentTransformRules::KeepRelativeTransform);

    _Text_ExitGame2 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_ExitGame2"));
    _Text_ExitGame2->SetText(FText::FromString("EXIT GAME"));
    _Text_ExitGame2->AttachToComponent(_Slot_ExitGame2, FAttachmentTransformRules::KeepRelativeTransform);
}

void AMenu::BeginPlay() {
    Super::BeginPlay();
}

void AMenu::InitMenu() {
    /*** MAIN MENU ***/
    _Slot_NewGameReleasedDelegate.BindUObject(this, &AMenu::BuildMenu_NewGame);
    _Slot_NewGame->AddOnInputMenuDelegate(_Slot_NewGameReleasedDelegate, false);

    _Slot_ExitGameReleasedDelegate.BindUObject(this, &AMenu::ExitGame);
    _Slot_ExitGame->AddOnInputMenuDelegate(_Slot_ExitGameReleasedDelegate, false);

    /*** NEW GAME MENU ***/
    _Slot_HostGame->AddOnInputMenuDelegate(_Slot_HostGameReleasedDelegate, false);

    _Slot_FindGameReleasedDelegate.BindUObject(this, &AMenu::BuildMenu_FindGame);
    _Slot_FindGame->AddOnInputMenuDelegate(_Slot_FindGameReleasedDelegate, false);

    _Slot_GoBackReleasedDelegate.BindUObject(this, &AMenu::BuildMenu_Main);
    _Slot_GoBack->AddOnInputMenuDelegate(_Slot_GoBackReleasedDelegate, false);

    /*** FIND GAME MENU ***/
    _Slot_JoinGame->AddOnInputMenuDelegate(_Slot_JoinGameReleasedDelegate, false);

    _Slot_GoBack2ReleasedDelegate.BindUObject(this, &AMenu::BuildMenu_NewGame);
    _Slot_GoBack2->AddOnInputMenuDelegate(_Slot_GoBack2ReleasedDelegate, false);

    /*** PLAY GAME MENU ***/
    _Slot_BackToMenu->AddOnInputMenuDelegate(_Slot_BackToMenuReleasedDelegate, false);

    _Slot_ExitGame2ReleasedDelegate.BindUObject(this, &AMenu::ExitGame);
    _Slot_ExitGame2->AddOnInputMenuDelegate(_Slot_ExitGame2ReleasedDelegate, false);

    BuildMenu_Main();
}

void AMenu::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

/*********************************** BINDINGS ****************************************************/
void AMenu::BuildMenu_Main() {
    ULibraryUtils::Log("BuildMenu_Main");
    EnableSubmenu(_Menu_NewGame, false);
    EnableSubmenu(_Menu_FindGame, false);
    EnableSubmenu(_Menu_PlayGame, false);
    EnableSubmenu(_Menu_Main, true);
}

void AMenu::BuildMenu_NewGame() {
    ULibraryUtils::Log("BuildMenu_NewGame");
    EnableSubmenu(_Menu_Main, false);
    EnableSubmenu(_Menu_FindGame, false);
    EnableSubmenu(_Menu_PlayGame, false);
    EnableSubmenu(_Menu_NewGame, true);
}

void AMenu::BuildMenu_FindGame() {
    ULibraryUtils::Log("BuildMenu_FindGame");
    EnableSubmenu(_Menu_Main, false);
    EnableSubmenu(_Menu_NewGame, false);
    EnableSubmenu(_Menu_PlayGame, false);
    EnableSubmenu(_Menu_FindGame, true);
}

void AMenu::BuildMenu_PlayGame() {
    ULibraryUtils::Log("BuildMenu_PlayGame");
    EnableSubmenu(_Menu_Main, false);
    EnableSubmenu(_Menu_NewGame, false);
    EnableSubmenu(_Menu_FindGame, false);
    EnableSubmenu(_Menu_PlayGame, true);
}

void AMenu::ExitGame() {
    ULibraryUtils::Log("ExitGame");
    FGenericPlatformMisc::RequestExit(false);
}

/*********************************** MENU TAB ****************************************************/
void AMenu::ResetMenu(bool InPlay) {
    ULibraryUtils::Log("ResetMenu");
    if (InPlay) BuildMenu_PlayGame();
    else BuildMenu_Main();
}

void AMenu::EnableSubmenu(USceneComponent* Submenu, bool Enable) {
    TArray<USceneComponent*> ChildComponents;
    Submenu->GetChildrenComponents(true, ChildComponents);

    for (USceneComponent* Component : ChildComponents) {
        Component->SetActive(Enable);
        Component->SetHiddenInGame(!Enable, true);
        Component->SetComponentTickEnabled(Enable);
        Component->SetVisibility(Enable, true);
        UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(Component);
        if (StaticMeshComp) {
            if(Enable) StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            else  StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}