// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "MenuPanel.generated.h"


class UInputMenu;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LTMV_API UMenuPanel : public USceneComponent {
    GENERATED_BODY()

public:
    UMenuPanel();
    void AddMenuInput(UInputMenu* NewSlot);

    void EnablePanel(bool Enable);
    UInputMenu* GetInputMenuAt(int Index);

protected:
    TArray<UInputMenu*> _MenuInputs;
};
