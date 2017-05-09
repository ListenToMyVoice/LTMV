// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "InputMenu.h"


UInputMenu::UInputMenu(const FObjectInitializer& OI) : Super(OI) {
    PrimaryComponentTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder(
        TEXT("/Game/Meshes/Static/Menu/menu_2_cajaprueba.menu_2_cajaprueba"));
    SetStaticMesh(Finder.Object);

    _Color = FColor::Black;
    _HoverColor = FColor::Red;

    _NextPoint = FVector();
    _NewTime = 0;
    _Timer = 0;

    //OnComponentActivated.AddDynamic(this, &UInputMenu::OnActivate);
    //OnComponentDeactivated.AddDynamic(this, &UInputMenu::OnDeactivate);
}

//void UInputMenu::OnActivate(UActorComponent* Component, bool bReset) {
//    ULibraryUtils::Log("OnActivate");
//}
//
//void UInputMenu::OnDeactivate(UActorComponent* Component) {
//    ULibraryUtils::Log("OnDeactivate");
//}

void UInputMenu::BeginPlay() {
    Super::BeginPlay();

    TArray<USceneComponent*> ChildrenComp;
    GetChildrenComponents(false, ChildrenComp);

    for (USceneComponent* Component : ChildrenComp) {
        _TextRender = Cast<UTextRenderComponent>(Component);
        if (_TextRender) {
            _TextRender->SetWorldSize(12);
            _TextRender->SetTextRenderColor(_Color);
            _TextRender->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
            _TextRender->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
            break;
        }
    }
}

void UInputMenu::TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    _Timer += DeltaTime;
    if (_Timer >= _NewTime) {
        _NewTime = _Timer + 0.5;

        _NextPoint.X = FMath::FRandRange(-1, 1);
        _NextPoint.Y = FMath::FRandRange(-1, 1);
        _NextPoint.Z = FMath::FRandRange(-1, 1);
    }
    AddRelativeLocation(_NextPoint.GetSafeNormal() * 1.5 * DeltaTime);
}

void UInputMenu::PressEvents() {
    EndhoverInteraction();
    _InputMenuPressedEvent.Broadcast();
}

void UInputMenu::ReleaseEvents() {
    HoverInteraction();
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