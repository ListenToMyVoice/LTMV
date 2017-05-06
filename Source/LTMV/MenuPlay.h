// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Menu.h"
#include "MenuPlay.generated.h"

class UInputMenu;

UCLASS()
class LTMV_API AMenuPlay : public AMenu {
    GENERATED_BODY()

public:
    AMenuPlay(const class FObjectInitializer& OI);

protected:
    /*** PLAY GAME MENU ***/
    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USceneComponent* _Menu_PlayGame;

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

    FORCEINLINE USceneComponent* AMenuPlay::GetMenu_PlayGame() const { return _Menu_PlayGame; }
    FORCEINLINE UInputMenu* AMenuPlay::GetSlot_BackToMenu() const { return _Slot_BackToMenu; }
    FORCEINLINE UTextRenderComponent* AMenuPlay::GetText_BackToMenu() const { return _Text_BackToMenu; }
    FORCEINLINE UInputMenu* AMenuPlay::GetSlot_ExitGame() const { return _Slot_ExitGame; }
    FORCEINLINE UTextRenderComponent* AMenuPlay::GetText_ExitGame() const { return _Text_ExitGame; }
};
