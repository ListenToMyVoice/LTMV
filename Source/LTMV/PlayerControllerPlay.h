// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayerControllerPlay.generated.h"


UCLASS()
class LTMV_API APlayerControllerPlay : public APlayerController {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    TSubclassOf<AActor> _MenuClass;

    APlayerControllerPlay(const FObjectInitializer& OI);

    virtual void BeginPlay() override;
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_CallUpdate(FPlayerInfo info);

protected:
    AActor* _MenuActor;

    virtual void SetupInputComponent() override;

    /********************************** ACTION MAPPINGS ******************************************/
    /*************** TRIGGER MENU *************/
    void ToogleMenu();

private:
    /***************** EXIT GAME **************/
    void ExitGame();
};