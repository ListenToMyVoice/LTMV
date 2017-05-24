// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Menu3D.h"

#include "MenuPanel.h"


AMenu3D::AMenu3D(const class FObjectInitializer& OI) : Super(OI) {
    PrimaryActorTick.bCanEverTick = true;

    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));

    /*** DECORATORS ***/
    _TopDecorator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("_TopDecorator"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder(
        TEXT("/Game/Meshes/Static/Menu/menu_2_parte_superior.menu_2_parte_superior"));
    _TopDecorator->SetStaticMesh(Finder.Object);

    _BottomDecorator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("_BottomDecorator"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder2(
        TEXT("/Game/Meshes/Static/Menu/menu_2_parte_abajo.menu_2_parte_abajo"));
    _BottomDecorator->SetStaticMesh(Finder2.Object);

    _MiddleDecorator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("_MiddleDecorator"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder3(
        TEXT("/Game/Meshes/Static/Menu/menu_2_parte_intermedia.menu_2_parte_intermedia"));
    _MiddleDecorator->SetStaticMesh(Finder3.Object);
    

    _TopDecorator->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    _BottomDecorator->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    _MiddleDecorator->AttachToComponent(_TopDecorator, FAttachmentTransformRules::KeepRelativeTransform,
                                        FName("SocketMiddle"));

    _IsMenuHidden = true;
    _Submenus = {};
}

void AMenu3D::AddSubmenu(UMenuPanel* Submenu) {
    Submenu->RegisterComponent();

    Submenu->AttachToComponent(_TopDecorator, FAttachmentTransformRules::KeepRelativeTransform);
    Submenu->RelativeLocation = _SubmenuLocation;
    Submenu->RelativeRotation = _SubmenuRotator;

    _Submenus.Add(Submenu);
}

void AMenu3D::ToogleMenu(FVector Location, FRotator Rotation) {
    if (_IsMenuHidden) {
        SetSubmenuByIndex(0);
        ULibraryUtils::SetActorEnable(this);

        SetActorLocationAndRotation(Location, Rotation, false, nullptr, ETeleportType::TeleportPhysics);
    }
    else {
        ULibraryUtils::SetActorEnable(this, false);
        SetSubmenuByIndex(-1);
    }
    _IsMenuHidden = !_IsMenuHidden;
}

void AMenu3D::SetSubmenuByIndex(int Index) {
    for (int i = 0; i < _Submenus.Num(); i++) {
        if (i == Index) {
            _Submenus[i]->EnablePanel(true);
            _BottomDecorator->AttachToComponent(_Submenus[i], FAttachmentTransformRules::KeepRelativeTransform);
            _BottomDecorator->RelativeLocation = FVector(0, 0, -50);
        }
        else {
            _Submenus[i]->EnablePanel(false);
        }
    }
}