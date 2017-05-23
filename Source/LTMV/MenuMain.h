// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Menu.h"
#include "MenuMain.generated.h"

class UInputMenu;

UCLASS()
class LTMV_API AMenuMain : public AMenu {
    GENERATED_BODY()

public:
    AMenuMain(const class FObjectInitializer& OI);
    void OnFindSessionComplete(FString SessionOwner) override;

protected:
    /*** MAIN MENU ***/
    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USceneComponent* _Menu_Main;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_NewGame;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_NewGame;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_Options;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_Options;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_ExitGame;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_ExitGame;

    /*** OPTIONS MENU ***/
    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USceneComponent* _Menu_Options;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_ComfortMode;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_ComfortMode;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_GoBack3;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_GoBack3;

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

    /*** FIND GAME MENU ***/
    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USceneComponent* _Menu_FindGame;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_JoinGame;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_JoinGame;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_GoBack2;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_GoBack2;

    virtual void BeginPlay() override;
    void BindDelegates() override;
    void ResetMenu() override;
    void DeactivateMenuMenu() override;
    void BuildLayout() override;

private:
    FInputMenuDelegate _Slot_HostGamePressedDelegate;
    FInputMenuDelegate _Slot_HostGameReleasedDelegate;

    FInputMenuDelegate _Slot_FindGamePressedDelegate;
    FInputMenuDelegate _Slot_FindGameReleasedDelegate;

    FInputMenuDelegate _Slot_JoinGamePressedDelegate;
    FInputMenuDelegate _Slot_JoinGameReleasedDelegate;

    FInputMenuDelegate _Slot_NewGamePressedDelegate;
    FInputMenuDelegate _Slot_NewGameReleasedDelegate;

    FInputMenuDelegate _Slot_OptionsPressedDelegate;
    FInputMenuDelegate _Slot_OptionsReleasedDelegate;

    FInputMenuDelegate _Slot_OptionsComfortModePressedDelegate;
    FInputMenuDelegate _Slot_OptionsComfortModeReleasedDelegate;

    FInputMenuDelegate _Slot_ExitGamePressedDelegate;
    FInputMenuDelegate _Slot_ExitGameReleasedDelegate;

    FInputMenuDelegate _Slot_GoBackPressedDelegate;
    FInputMenuDelegate _Slot_GoBackReleasedDelegate;

    FInputMenuDelegate _Slot_GoBack2PressedDelegate;
    FInputMenuDelegate _Slot_GoBack2ReleasedDelegate;

    FInputMenuDelegate _Slot_GoBack3PressedDelegate;
    FInputMenuDelegate _Slot_GoBack3ReleasedDelegate;

    /*** BINDINGS ***/
    void BuildMenu_Main();
    void BuildMenu_Options();
    void BuildMenu_NewGame();
    void BuildMenu_FindGame();
    /* OPTIONS */
    void OptionComfortMode();

public:
    FORCEINLINE USceneComponent* AMenuMain::GetMenu_Main() const { return _Menu_Main; }
    FORCEINLINE UInputMenu* AMenuMain::GetSlot_NewGame() const { return _Slot_NewGame; }
    FORCEINLINE UTextRenderComponent* AMenuMain::GetText_NewGame() const { return _Text_NewGame; }
    FORCEINLINE UInputMenu* AMenuMain::GetSlot_ExitGame() const { return _Slot_ExitGame; }
    FORCEINLINE UTextRenderComponent* AMenuMain::GetText_ExitGame() const { return _Text_ExitGame; }

    FORCEINLINE USceneComponent* AMenuMain::GetMenu_NewGame() const { return _Menu_NewGame; }
    FORCEINLINE UInputMenu* AMenuMain::GetSlot_HostGame() const { return _Slot_HostGame; }
    FORCEINLINE UTextRenderComponent* AMenuMain::GetText_HostGame() const { return _Text_HostGame; }
    FORCEINLINE UInputMenu* AMenuMain::GetSlot_FindGame() const { return _Slot_FindGame; }
    FORCEINLINE UTextRenderComponent* AMenuMain::GetText_FindGame() const { return _Text_FindGame; }
    FORCEINLINE UInputMenu* AMenuMain::GetSlot_GoBack() const { return _Slot_GoBack; }
    FORCEINLINE UTextRenderComponent* AMenuMain::GetText_GoBack() const { return _Text_GoBack; }

    FORCEINLINE USceneComponent* AMenuMain::GetMenu_FindGame() const { return _Menu_FindGame; }
    FORCEINLINE UInputMenu* AMenuMain::GetSlot_JoinGame() const { return _Slot_JoinGame; }
    FORCEINLINE UTextRenderComponent* AMenuMain::GetText_JoinGame() const { return _Text_JoinGame; }
    FORCEINLINE UInputMenu* AMenuMain::GetSlot_GoBack2() const { return _Slot_GoBack2; }
    FORCEINLINE UTextRenderComponent* AMenuMain::GetText_GoBack2() const { return _Text_GoBack2; }
};
