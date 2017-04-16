// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NWGameInstance.h"

#include "GameFramework/PlayerController.h"
#include "PlayerControllerLobby.generated.h"


UCLASS()
class LTMV_API APlayerControllerLobby : public APlayerController {
    GENERATED_BODY()

public:
    APlayerControllerLobby(const FObjectInitializer& OI);

    UFUNCTION(Client, Reliable)
    void CLIENT_InitialSetup();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_CallUpdate(FPlayerInfo info, bool changedStatus);


    UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Menu")
    void CLIENT_CreateMenu(TSubclassOf<AActor> menuClass);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    TSubclassOf<AActor> _MenuClass;
    AActor* _MenuActor;
    bool _IsMenuHidden;

    virtual void SetupInputComponent() override;

    /********************************** ACTION MAPPINGS ******************************************/
    /*************** TRIGGER MENU *************/
    void ToogleMenu();

private:
    FString _MapMainMenu;

    /********************************** ACTION MAPPINGS ******************************************/
    /***************** EXIT GAME **************/
    void ExitGame();
};
