// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Walkie.h"


UWalkie::UWalkie() {}

void UWalkie::BeginPlay() {
    Super::BeginPlay();
    GetOwner()->SetReplicates(true);
}

void UWalkie::UseItemPressed_Implementation() {
    _RadioPressedEvent.Broadcast();
}


void UWalkie::UseItemReleased_Implementation() {
    _RadioReleasedEvent.Broadcast();
}

/*********************************************** DELEGATES ***************************************/
FDelegateHandle UWalkie::AddOnRadioDelegate(FRadioDelegate& RadioDelegate, bool IsPressed) {
    if(IsPressed) return _RadioPressedEvent.Add(RadioDelegate);
    else return _RadioReleasedEvent.Add(RadioDelegate);
}

void UWalkie::ClearOnRadioDelegate(FDelegateHandle DelegateHandle, bool IsPressed) {
    if (IsPressed) _RadioPressedEvent.Remove(DelegateHandle);
    else _RadioReleasedEvent.Remove(DelegateHandle);
}