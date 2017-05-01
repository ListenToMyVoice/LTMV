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

protected:
    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_NewGame;
    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_NewGame;

    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UInputMenu* _Slot_ExitGame;
    UPROPERTY(Category = "Menu Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* _Text_ExitGame;

	virtual void BeginPlay() override;

private:
    FInputMenuDelegate _Slot_NewGamePressedDelegate;
    FInputMenuDelegate _Slot_NewGameReleasedDelegate;

    FInputMenuDelegate _Slot_ExitGamePressedDelegate;
    FInputMenuDelegate _Slot_ExitGameReleasedDelegate;

    void NewGame();
    void ExitGame();

public:
    FORCEINLINE UInputMenu* AMenu::GetSlot_NewGame() const { return _Slot_NewGame; }
    FORCEINLINE UTextRenderComponent* AMenu::GetText_NewGame() const { return _Text_NewGame; }
};
