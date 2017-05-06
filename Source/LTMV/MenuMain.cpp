// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "MenuMain.h"

#include "InputMenu.h"
#include "NWGameInstance.h"

AMenuMain::AMenuMain(const class FObjectInitializer& OI) : Super(OI) {
    /************************************ MAIN MENU **********************************************/
    _Menu_Main = CreateDefaultSubobject<USceneComponent>(TEXT("_Menu_Main"));
    _Menu_Main->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_NewGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_NewGame"));
    _Slot_NewGame->AttachToComponent(_Menu_Main, FAttachmentTransformRules::KeepRelativeTransform);
    _Text_NewGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_NewGame"));
    _Text_NewGame->SetText(FText::FromString("NEW GAME"));
    _Text_NewGame->AttachToComponent(_Slot_NewGame, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketText"));

    _Slot_ExitGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_ExitGame"));
    _Slot_ExitGame->AttachToComponent(_Menu_Main, FAttachmentTransformRules::KeepRelativeTransform);
    _Text_ExitGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_ExitGame"));
    _Text_ExitGame->SetText(FText::FromString("EXIT GAME"));
    _Text_ExitGame->AttachToComponent(_Slot_ExitGame, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketText"));


    /*************************************** NEW GAME MENU ***************************************/
    _Menu_NewGame = CreateDefaultSubobject<USceneComponent>(TEXT("_Menu_NewGame"));
    _Menu_NewGame->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_HostGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_HostGame"));
    _Slot_HostGame->AttachToComponent(_Menu_NewGame, FAttachmentTransformRules::KeepRelativeTransform);
    _Text_HostGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_HostGame"));
    _Text_HostGame->SetText(FText::FromString("HOST GAME"));
    _Text_HostGame->AttachToComponent(_Slot_HostGame, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketText"));

    _Slot_FindGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_FindGame"));
    _Slot_FindGame->AttachToComponent(_Menu_NewGame, FAttachmentTransformRules::KeepRelativeTransform);
    _Text_FindGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_FindGame"));
    _Text_FindGame->SetText(FText::FromString("FIND GAME"));
    _Text_FindGame->AttachToComponent(_Slot_FindGame, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketText"));

    _Slot_GoBack = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_GoBack"));
    _Slot_GoBack->AttachToComponent(_Menu_NewGame, FAttachmentTransformRules::KeepRelativeTransform);
    _Text_GoBack = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_GoBack"));
    _Text_GoBack->SetText(FText::FromString("GO BACK"));
    _Text_GoBack->AttachToComponent(_Slot_GoBack, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketText"));


    /*************************************** FIND GAME MENU **************************************/
    _Menu_FindGame = CreateDefaultSubobject<USceneComponent>(TEXT("_Menu_FindGame"));
    _Menu_FindGame->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

    _Slot_JoinGame = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_JoinGame"));
    _Slot_JoinGame->AttachToComponent(_Menu_FindGame, FAttachmentTransformRules::KeepRelativeTransform);
    _Text_JoinGame = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_JoinGame"));
    _Text_JoinGame->SetText(FText::FromString("JOIN GAME"));
    _Text_JoinGame->AttachToComponent(_Slot_JoinGame, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketText"));

    _Slot_GoBack2 = CreateDefaultSubobject<UInputMenu>(TEXT("_Slot_GoBack2"));
    _Slot_GoBack2->AttachToComponent(_Menu_FindGame, FAttachmentTransformRules::KeepRelativeTransform);
    _Text_GoBack2 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_Text_GoBack2"));
    _Text_GoBack2->SetText(FText::FromString("GO BACK"));
    _Text_GoBack2->AttachToComponent(_Slot_GoBack2, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketText"));

    BuildLayout();
}

void AMenuMain::BuildLayout() {
    Super::BuildLayout();

    //FVector Origin;
    //FVector BoxExtent;
    //float SphereRadius;
    //UKismetSystemLibrary::GetComponentBounds(_Slot_NewGame, Origin, BoxExtent, SphereRadius);
    //ULibraryUtils::Log(FString::Printf(TEXT("Origin: X= %d Y= %d Z= %d"), Origin.X, Origin.Y, Origin.Z));
    //ULibraryUtils::Log(FString::Printf(TEXT("BoxExtent: X= %d Y= %d Z= %d"), BoxExtent.X, BoxExtent.Y, BoxExtent.Z));

    float VerticalLocation = 0;
    /************************************ MAIN MENU **********************************************/
    _Menu_Main->RelativeLocation = _SubmenuLocation;
    _Menu_Main->RelativeRotation = _SubmenuRotator;

    _Slot_NewGame->RelativeLocation = FVector(0, 0, VerticalLocation);
    VerticalLocation -= MeshHeight;
    _Slot_ExitGame->RelativeLocation = FVector(0, 0, VerticalLocation);

    VerticalLocation = 0;
    /*************************************** NEW GAME MENU ***************************************/
    _Menu_NewGame->RelativeLocation = _SubmenuLocation;
    _Menu_NewGame->RelativeRotation = _SubmenuRotator;

    _Slot_HostGame->RelativeLocation = FVector(0, 0, VerticalLocation);
    VerticalLocation -= MeshHeight;
    _Slot_FindGame->RelativeLocation = FVector(0, 0, VerticalLocation);
    VerticalLocation -= MeshHeight;
    _Slot_GoBack->RelativeLocation = FVector(0, 0, VerticalLocation);

    VerticalLocation = 0;
    /*************************************** FIND GAME MENU **************************************/
    _Menu_FindGame->RelativeLocation = _SubmenuLocation;
    _Menu_FindGame->RelativeRotation = _SubmenuRotator;

    _Slot_JoinGame->RelativeLocation = FVector(0, 0, VerticalLocation);
    VerticalLocation -= MeshHeight;
    _Slot_GoBack2->RelativeLocation = FVector(0, 0, VerticalLocation);

    ///* DECORATORS */
    //_TopDecorator->AttachToComponent(_Slot_NewGame, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketTop"));
    //_BottomDecorator->AttachToComponent(_Slot_GoBack2, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketBottom"));
}

void AMenuMain::BeginPlay() {
    Super::BeginPlay();
    
    BindDelegates();
}

void AMenuMain::BindDelegates() {
    /*** MAIN MENU ***/
    _Slot_NewGameReleasedDelegate.BindUObject(this, &AMenuMain::BuildMenu_NewGame);
    _Slot_NewGame->AddOnInputMenuDelegate(_Slot_NewGameReleasedDelegate, false);

    _Slot_ExitGameReleasedDelegate.BindUObject(this, &AMenu::OnExitGame);
    _Slot_ExitGame->AddOnInputMenuDelegate(_Slot_ExitGameReleasedDelegate, false);

    /*** NEW GAME MENU ***/
    _Slot_HostGameReleasedDelegate.BindUObject(this, &AMenu::OnHostSession);
    _Slot_HostGame->AddOnInputMenuDelegate(_Slot_HostGameReleasedDelegate, false);

    _Slot_FindGameReleasedDelegate.BindUObject(this, &AMenuMain::BuildMenu_FindGame);
    _Slot_FindGame->AddOnInputMenuDelegate(_Slot_FindGameReleasedDelegate, false);

    _Slot_GoBackReleasedDelegate.BindUObject(this, &AMenuMain::BuildMenu_Main);
    _Slot_GoBack->AddOnInputMenuDelegate(_Slot_GoBackReleasedDelegate, false);

    /*** FIND GAME MENU ***/
    _Slot_JoinGameReleasedDelegate.BindUObject(this, &AMenu::OnJoinGame);
    _Slot_JoinGame->AddOnInputMenuDelegate(_Slot_JoinGameReleasedDelegate, false);

    _Slot_GoBack2ReleasedDelegate.BindUObject(this, &AMenuMain::BuildMenu_NewGame);
    _Slot_GoBack2->AddOnInputMenuDelegate(_Slot_GoBack2ReleasedDelegate, false);
}

void AMenuMain::ResetMenu() {
    BuildMenu_Main();
}

void AMenuMain::DeactivateMenuMenu() {
    EnableSubmenu(_Menu_NewGame, false);
    EnableSubmenu(_Menu_FindGame, false);
    EnableSubmenu(_Menu_Main, false);
}

/*********************************** BINDINGS ****************************************************/
void AMenuMain::BuildMenu_Main() {
    EnableSubmenu(_Menu_NewGame, false);
    EnableSubmenu(_Menu_FindGame, false);

    /* DECORATORS */
    _TopDecorator->AttachToComponent(_Slot_NewGame, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketTop"));
    _BottomDecorator->AttachToComponent(_Slot_ExitGame, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketBottom"));
    EnableSubmenu(_Menu_Main, true);
}

void AMenuMain::BuildMenu_NewGame() {
    EnableSubmenu(_Menu_Main, false);
    EnableSubmenu(_Menu_FindGame, false);

    /* DECORATORS */
    _TopDecorator->AttachToComponent(_Slot_HostGame, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketTop"));
    _BottomDecorator->AttachToComponent(_Slot_GoBack, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketBottom"));
    EnableSubmenu(_Menu_NewGame, true);
}

void AMenuMain::BuildMenu_FindGame() {
    OnFindGame();

    EnableSubmenu(_Menu_Main, false);
    EnableSubmenu(_Menu_NewGame, false);

    /* DECORATORS */
    _TopDecorator->AttachToComponent(_Slot_JoinGame, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketTop"));
    _BottomDecorator->AttachToComponent(_Slot_GoBack2, FAttachmentTransformRules::KeepRelativeTransform, FName("SocketBottom"));
    EnableSubmenu(_Menu_FindGame, true);

    _Slot_JoinGame->SetActive(false);
    _Slot_JoinGame->SetHiddenInGame(true, true);
    _Slot_JoinGame->SetComponentTickEnabled(false);
    _Slot_JoinGame->SetVisibility(false, true);
    _Slot_JoinGame->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMenuMain::OnFindSessionComplete(FString SessionOwner) {
    ULibraryUtils::Log(SessionOwner);

    _Slot_JoinGame->SetActive(true);
    _Slot_JoinGame->SetHiddenInGame(false, true);
    _Slot_JoinGame->SetComponentTickEnabled(true);
    _Slot_JoinGame->SetVisibility(true, true);
    _Slot_JoinGame->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}