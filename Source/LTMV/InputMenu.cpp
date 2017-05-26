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

    _TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("_TextRender"));
    _TextRender->SetWorldSize(12);
    _TextRender->SetTextRenderColor(_Color);
    _TextRender->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    _TextRender->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);

    _NextPoint = FVector();
    _NewTime = 0;
    _Timer = 0;

    _IsLoading = false;

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
    _TextRender->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform,
                                   FName("SocketText"));
    _TextRender->SetText(FText::FromString(GetFName().ToString()));
    _TextRender->RegisterComponent();
}

void UInputMenu::TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    //_Timer += DeltaTime;
    //if (_Timer >= _NewTime) {
    //    _NewTime = _Timer + 0.5;

    //    _NextPoint.X = FMath::FRandRange(-1, 1);
    //    _NextPoint.Y = FMath::FRandRange(-1, 1);
    //    _NextPoint.Z = FMath::FRandRange(-1, 1);
    //}
    //AddRelativeLocation(_NextPoint.GetSafeNormal() * 1.5 * DeltaTime);

    /* LOADING */
    if (_IsLoading) {
        FRotator NewRotation = FRotator(0, 0, 1.5 * DeltaTime);
        AddRelativeRotation(NewRotation);
    }
}

void UInputMenu::PressEvents() {
    if (!_IsLoading) {
        EndhoverInteraction();
        _InputMenuPressedEvent.Broadcast(this);
    }
}

void UInputMenu::ReleaseEvents() {
    if (!_IsLoading) {
        HoverInteraction();
        _InputMenuReleasedEvent.Broadcast(this);
    }
}

void UInputMenu::HoverInteraction() {
    if (_TextRender) _TextRender->SetTextRenderColor(_HoverColor);
}

void UInputMenu::EndhoverInteraction() {
    if (_TextRender) _TextRender->SetTextRenderColor(_Color);
}

void UInputMenu::Enable(bool Enable) {
    SetActive(Enable);
    SetHiddenInGame(!Enable, true);
    SetComponentTickEnabled(Enable);
    SetVisibility(Enable, true);
    if (Enable) SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    else SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UInputMenu::SetLoading(bool IsLoading, FString Text) {
    _IsLoading = IsLoading;
    if (_IsLoading) {
        _PrevText = _TextRender->Text;
        _TextRender->SetText(FText::FromString(Text));
        ULibraryUtils::Log("LOADING");
    }
    else {
        _TextRender->SetText(_PrevText);
        SetRelativeRotation(FRotator(0, 0, 0));
        ULibraryUtils::Log("END LOADING");
    }

    //if (_IsLoading) {
    //    _PrevText = _TextRender->Text;
    //    _TextRender->SetText(FText::FromString(Text));
    //}
    //else {
    //    _TextRender->SetText(_PrevText);
    //}
}

/*********************************************** DELEGATES ***************************************/
void UInputMenu::AddOnInputMenuDelegate() {
    if(_InputMenuPressedDelegate.IsBound())
        _OnInputMenuPressedDelegateHandle = _InputMenuPressedEvent.Add(_InputMenuPressedDelegate);
    
    if (_InputMenuReleasedDelegate.IsBound())
        _OnInputMenuReleasedDelegateHandle = _InputMenuReleasedEvent.Add(_InputMenuReleasedDelegate);
}

void UInputMenu::ClearOnInputMenuDelegate() {
    _InputMenuPressedEvent.Remove(_OnInputMenuPressedDelegateHandle);
    _InputMenuReleasedEvent.Remove(_OnInputMenuReleasedDelegateHandle);
}