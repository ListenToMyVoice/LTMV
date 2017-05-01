// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Menu.generated.h"

class UInputMenu;

UCLASS()
class LTMV_API AMenu : public AActor {
	GENERATED_BODY()
	
public:
    UInputMenu* _Slot_NewGame;
    UTextRenderComponent* _Text_NewGame;
    FInputMenuDelegate _Slot_NewGamePressedDelegate;
    FInputMenuDelegate _Slot_NewGameReleasedDelegate;

    //UInputMenu* _Slot_Options;
    //UInputMenu* _Slot_ExitGame;

	AMenu();
    virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
    /***************** EXIT GAME **************/
    void ExitGame();
};
