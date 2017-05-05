// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Menu.generated.h"


UCLASS()
class LTMV_API AMenu : public AActor {
	GENERATED_BODY()

public:
    bool _IsMenuHidden;

	AMenu(const class FObjectInitializer& OI);
    void ToogleMenu(FVector Location, FRotator Rotation);

    /*** BINDINGS ***/
    void OnExitGame();
    /*** GAME INSTANCE ***/
    void OnHostSession();
    void OnFindGame();
    void OnJoinGame();
    void OnBackToMenu();

    virtual void OnFindSessionComplete(FString SessionOwner);

    /*** MENU TAB ***/
    void EnableSubmenu(USceneComponent* Submenu, bool Enable);

protected:
    float MeshHeight = 40;
    float MeshWidth = 110;
    float MeshDepth = 55;

    virtual void BindDelegates();
    virtual void ResetMenu();
    virtual void BuildLayout();
};
