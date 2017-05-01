// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Menu.generated.h"

class UInputMenu;

UCLASS()
class LTMV_API AMenu : public AActor {
	GENERATED_BODY()
	
public:
	AMenu();
    virtual void Tick(float DeltaTime) override;

    /*** BINDINGS ***/
    void BuildMenu_Main();

protected:
    /*** MAIN MENU ***/
    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USceneComponent* _Menu_Main;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_NewGame;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_NewGame;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_ExitGame;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_ExitGame;

    /*** NEW GAME MENU ***/
    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USceneComponent* _Menu_NewGame;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_HostGame;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_HostGame;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_FindGame;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_FindGame;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_GoBack;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_GoBack;

	virtual void BeginPlay() override;

private:
    FInputMenuDelegate _Slot_NewGamePressedDelegate;
    FInputMenuDelegate _Slot_NewGameReleasedDelegate;

    FInputMenuDelegate _Slot_ExitGamePressedDelegate;
    FInputMenuDelegate _Slot_ExitGameReleasedDelegate;

    FInputMenuDelegate _Slot_HostGamePressedDelegate;
    FInputMenuDelegate _Slot_HostGameReleasedDelegate;

    FInputMenuDelegate _Slot_FindGamePressedDelegate;
    FInputMenuDelegate _Slot_FindGameReleasedDelegate;

    FInputMenuDelegate _Slot_GoBackPressedDelegate;
    FInputMenuDelegate _Slot_GoBackReleasedDelegate;

    /*** BINDINGS ***/
    void BuildMenu_NewGame();
    void ExitGame();

    void HostGame();
    void FindGame();

    /*** MENU TAB ***/
    void EnableSubmenu(USceneComponent* Submenu, bool Enable);

public:
    FORCEINLINE USceneComponent* AMenu::GetMenu_Main() const { return _Menu_Main; }
    FORCEINLINE UInputMenu* AMenu::GetSlot_NewGame() const { return _Slot_NewGame; }
    FORCEINLINE UTextRenderComponent* AMenu::GetText_NewGame() const { return _Text_NewGame; }
    FORCEINLINE UInputMenu* AMenu::GetSlot_ExitGame() const { return _Slot_ExitGame; }
    FORCEINLINE UTextRenderComponent* AMenu::GetText_ExitGame() const { return _Text_ExitGame; }

    FORCEINLINE USceneComponent* AMenu::GetMenu_NewGame() const { return _Menu_NewGame; }
    FORCEINLINE UInputMenu* AMenu::GetSlot_HostGame() const { return _Slot_HostGame; }
    FORCEINLINE UTextRenderComponent* AMenu::GetText_HostGame() const { return _Text_HostGame; }
    FORCEINLINE UInputMenu* AMenu::GetSlot_FindGame() const { return _Slot_FindGame; }
    FORCEINLINE UTextRenderComponent* AMenu::GetText_FindGame() const { return _Text_FindGame; }
    FORCEINLINE UInputMenu* AMenu::GetSlot_GoBack() const { return _Slot_GoBack; }
    FORCEINLINE UTextRenderComponent* AMenu::GetText_GoBack() const { return _Text_GoBack; }
};
