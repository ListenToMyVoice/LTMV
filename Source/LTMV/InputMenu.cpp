// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "InputMenu.h"


UInputMenu::UInputMenu(const FObjectInitializer& OI) : Super(OI) {
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder(
        TEXT("/Engine/BasicShapes/Cube"));
    SetStaticMesh(Finder.Object);

    _Color = FColor::Yellow;
    _HoverColor = FColor::Cyan;

    SetRelativeScale3D(FVector(0.2, 1.2, 0.3));
}

void UInputMenu::BeginPlay() {
    Super::BeginPlay();
    _TextRender = Cast<UTextRenderComponent>(GetChildComponent(0));
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