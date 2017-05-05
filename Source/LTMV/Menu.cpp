// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "Menu.h"
#include "NWGameInstance.h"


AMenu::AMenu(const class FObjectInitializer& OI) : Super(OI) {
    PrimaryActorTick.bCanEverTick = true;
    _IsMenuHidden = true;
    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root Component")));
}

void AMenu::BindDelegates() {}

void AMenu::ResetMenu() {}

void AMenu::BuildLayout() {}

void AMenu::OnFindSessionComplete(FString SessionOwner) {}

void AMenu::ToogleMenu(FVector Location, FRotator Rotation) {
    if (_IsMenuHidden) {
        ResetMenu();
        ULibraryUtils::SetActorEnable(this);
        
        SetActorLocationAndRotation(Location, Rotation, false, nullptr, ETeleportType::TeleportPhysics);
    }
    else {
        ULibraryUtils::SetActorEnable(this, false);
    }
    _IsMenuHidden = !_IsMenuHidden;
}

/*********************************** BINDINGS ****************************************************/
void AMenu::OnExitGame() {
    FGenericPlatformMisc::RequestExit(false);
}

/*** GAME INSTANCE ***/
void AMenu::OnBackToMenu() {
    UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetWorld()->GetGameInstance());
    if(GameInstance) GameInstance->DestroySession();
}

void AMenu::OnHostSession() {
    UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance) GameInstance->LaunchLobby();
}

void AMenu::OnFindGame() {
    UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance) GameInstance->FindOnlineGames();
}

void AMenu::OnJoinGame() {
    UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance) GameInstance->JoinOnlineGame();
}

/*********************************** MENU TAB ****************************************************/
void AMenu::EnableSubmenu(USceneComponent* Submenu, bool Enable) {
    TArray<USceneComponent*> ChildComponents;
    Submenu->GetChildrenComponents(true, ChildComponents);

    for (USceneComponent* Component : ChildComponents) {
        Component->SetActive(Enable);
        Component->SetHiddenInGame(!Enable, true);
        Component->SetComponentTickEnabled(Enable);
        Component->SetVisibility(Enable, true);
        UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(Component);
        if (StaticMeshComp) {
            if(Enable) StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            else  StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}