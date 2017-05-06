// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "InputMenu.h"


UInputMenu::UInputMenu(const FObjectInitializer& OI) : Super(OI) {
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder(
        TEXT("/Game/Meshes/Static/Menu/menu_2_cajaprueba.menu_2_cajaprueba"));
    SetStaticMesh(Finder.Object);

    _Color = FColor::Black;
    _HoverColor = FColor::Red;

    //SetRelativeScale3D(FVector(0.2, 1.2, 0.3));
}

void UInputMenu::BeginPlay() {
    Super::BeginPlay();
    _TextRender = Cast<UTextRenderComponent>(GetChildComponent(0));
    if (_TextRender) {
        _TextRender->SetWorldSize(18);
        _TextRender->SetTextRenderColor(_Color);
        _TextRender->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
        _TextRender->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
    }
}

void UInputMenu::PressEvents() {
    _InputMenuPressedEvent.Broadcast();
}

void UInputMenu::ReleaseEvents() {
    _InputMenuReleasedEvent.Broadcast();
}

void UInputMenu::HoverInteraction() {
    if (_TextRender) _TextRender->SetTextRenderColor(_HoverColor);
}

void UInputMenu::EndhoverInteraction() {
    if (_TextRender) _TextRender->SetTextRenderColor(_Color);
}

/*********************************************** DELEGATES ***************************************/
void UInputMenu::AddOnInputMenuDelegate(FInputMenuDelegate& InputMenuDelegate, bool IsPressed) {
    if (IsPressed) {
        _OnInputMenuPressedDelegateHandle = _InputMenuPressedEvent.Add(InputMenuDelegate);
    }
    else {
        _OnInputMenuReleasedDelegateHandle = _InputMenuReleasedEvent.Add(InputMenuDelegate);
    }
}

void UInputMenu::ClearOnInputMenuDelegate(bool IsPressed) {
    if (IsPressed) _InputMenuPressedEvent.Remove(_OnInputMenuPressedDelegateHandle);
    else _InputMenuReleasedEvent.Remove(_OnInputMenuReleasedDelegateHandle);
}