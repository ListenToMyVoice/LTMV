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

    FDelegateHandle _OnInputMenuPressedDelegateHandle;
    FDelegateHandle _OnInputMenuReleasedDelegateHandle;

    UInputMenu(const FObjectInitializer& OI);

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    void AddOnInputMenuDelegate(FInputMenuDelegate& InputMenuDelegate, bool IsPressed);
    void ClearOnInputMenuDelegate(bool IsPressed);

    void HoverInteraction();
    void EndhoverInteraction();

    void ReleaseEvents();
    void PressEvents();

    //UFUNCTION()
    //void OnActivate(UActorComponent* Component, bool bReset);
    //UFUNCTION()
    //void OnDeactivate(UActorComponent* Component);

protected:
    virtual void BeginPlay() override;

private:
    UTextRenderComponent* _TextRender;
    float _NewTime;
    float _Timer;

    FVector _NextPoint;
};
