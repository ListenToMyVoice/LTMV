// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Menu.h"
#include "MenuLobby.generated.h"

class UInputMenu;

UCLASS()
class LTMV_API AMenuLobby : public AMenu {
    GENERATED_BODY()

public:
    AMenuLobby(const class FObjectInitializer& OI);

protected:
    /*** LOBBY MENU ***/
    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USceneComponent* _Menu_Lobby;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_BackToMenu;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_BackToMenu;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_ExitGame;
    UPROPERTY(Category = "Menu Text", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_ExitGame;

    virtual void BeginPlay() override;
    void BindDelegates() override;
    void BuildLayout() override;
    void ResetMenu() override;
    void DeactivateMenuMenu() override;

private:
    FInputMenuDelegate _Slot_BackToMenuPressedDelegate;
    FInputMenuDelegate _Slot_BackToMenuReleasedDelegate;

    FInputMenuDelegate _Slot_ExitGamePressedDelegate;
    FInputMenuDelegate _Slot_ExitGameReleasedDelegate;

public:

    FORCEINLINE USceneComponent* AMenuLobby::GetMenu_Lobby() const { return _Menu_Lobby; }
    FORCEINLINE UInputMenu* AMenuLobby::GetSlot_BackToMenu() const { return _Slot_BackToMenu; }
    FORCEINLINE UTextRenderComponent* AMenuLobby::GetText_BackToMenu() const { return _Text_BackToMenu; }
    FORCEINLINE UInputMenu* AMenuLobby::GetSlot_ExitGame() const { return _Slot_ExitGame; }
    FORCEINLINE UTextRenderComponent* AMenuLobby::GetText_ExitGame() const { return _Text_ExitGame; }
};
