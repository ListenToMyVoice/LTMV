// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "MenuPanel.h"

#include "InputMenu.h"

UMenuPanel::UMenuPanel() : Super() {
    _MenuInputs = {};
    _PanelHeight = 0;
}

void UMenuPanel::AddMenuInput(UInputMenu* NewSlot) {
    NewSlot->RegisterComponent();

    //if (_MenuInputs.Num() == 0) {
    //    NewSlot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
    //    NewSlot->RelativeLocation = FVector(0, 0, 0);
    //}
    //else {
    //    NewSlot->AttachToComponent(_MenuInputs.Top(), FAttachmentTransformRules::KeepRelativeTransform);
    //    NewSlot->RelativeLocation = FVector(0, 0, -NewSlot->_MeshHeight);
    //}

    NewSlot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
    NewSlot->RelativeLocation = FVector(0, 0, -NewSlot->_MeshHeight*_MenuInputs.Num());

    _MenuInputs.Add(NewSlot);
    _PanelHeight += NewSlot->_MeshHeight;
}

void UMenuPanel::EnablePanel(bool Enable) {
    for (UInputMenu* InputMenu : _MenuInputs) {
        InputMenu->Enable(Enable);
    }
}

UInputMenu* UMenuPanel::GetInputMenuAt(int Index) {
    return Index < _MenuInputs.Num() ? _MenuInputs[Index] : nullptr;

}

UInputMenu* UMenuPanel::GetInputMenuLast() {
    return _MenuInputs.Top();
}