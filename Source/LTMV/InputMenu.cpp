// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "InputMenu.h"


UInputMenu::UInputMenu() : Super() {}

void UInputMenu::PressEvents() {
    _InputMenuPressedEvent.Broadcast();
}


void UInputMenu::ReleaseEvents() {
    _InputMenuReleasedEvent.Broadcast();
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