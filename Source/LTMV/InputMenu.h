// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "InputMenu.generated.h"

DECLARE_EVENT(UInputMenu, FInputMenuEvent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LTMV_API UInputMenu : public UStaticMeshComponent {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    FColor _Color;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    FColor _HoverColor;

    FInputMenuEvent _InputMenuPressedEvent;
    FInputMenuEvent _InputMenuReleasedEvent;

    /* Radio Delegate */
    FDelegateHandle _OnInputMenuPressedDelegateHandle;
    FDelegateHandle _OnInputMenuReleasedDelegateHandle;

    UInputMenu(const FObjectInitializer& OI);

    void AddOnInputMenuDelegate(FInputMenuDelegate& InputMenuDelegate, bool IsPressed);
    void ClearOnInputMenuDelegate(bool IsPressed);

    void HoverInteraction();
    void EndhoverInteraction();

    void ReleaseEvents();
    void PressEvents();

protected:
    virtual void BeginPlay() override;

private:
    UTextRenderComponent* _TextRender;
};
