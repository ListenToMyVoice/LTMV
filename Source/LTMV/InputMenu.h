// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "InputMenu.generated.h"

DECLARE_EVENT(UInputMenu, FInputMenuEvent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LTMV_API UInputMenu : public UStaticMeshComponent {
    GENERATED_BODY()

public:
    FInputMenuEvent _InputMenuPressedEvent;
    FInputMenuEvent _InputMenuReleasedEvent;

    //FInputMenuDelegate _OnInputMenuPressedDelegate;
    //FInputMenuDelegate _OnInputMenuReleasedDelegate;

    /* Radio Delegate */
    FDelegateHandle _OnInputMenuPressedDelegateHandle;
    FDelegateHandle _OnInputMenuReleasedDelegateHandle;

    UInputMenu();

    void AddOnInputMenuDelegate(FInputMenuDelegate& InputMenuDelegate, bool IsPressed);
    void ClearOnInputMenuDelegate(bool IsPressed);

    void ReleaseEvents();
    void PressEvents();
};
