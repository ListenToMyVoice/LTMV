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

    if (_MenuInputs.Num() == 0) {
        NewSlot->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
        NewSlot->RelativeLocation = FVector(0, 0, 0);
    }
    else {
        NewSlot->AttachToComponent(_MenuInputs.Top(), FAttachmentTransformRules::KeepRelativeTransform);
        NewSlot->RelativeLocation = FVector(0, 0, -NewSlot->_MeshHeight);
    }

    _MenuInputs.Add(NewSlot);
    _PanelHeight += NewSlot->_MeshHeight;
}

void UMenuPanel::EnablePanel(bool Enable) {
    for (UInputMenu* InputMenu : _MenuInputs) {
        InputMenu->SetActive(Enable);
        InputMenu->SetHiddenInGame(!Enable, true);
        InputMenu->SetComponentTickEnabled(Enable);
        InputMenu->SetVisibility(Enable, true);
        if (Enable) InputMenu->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        else  InputMenu->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

UInputMenu* UMenuPanel::GetInputMenuAt(int Index) {
    UInputMenu* Input;
    _MenuInputs.Find(Input, Index);
    return Input;

}

UInputMenu* UMenuPanel::GetInputMenuLast() {
    return _MenuInputs.Top();

}