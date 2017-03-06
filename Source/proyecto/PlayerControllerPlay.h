// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayerControllerPlay.generated.h"


UCLASS()
class PROYECTO_API APlayerControllerPlay : public APlayerController {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Player Info")
    FPlayerInfo _PlayerInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    TSubclassOf<AActor> _MenuClass;

    APlayerControllerPlay(const FObjectInitializer& OI);
    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_PassInfoToServer(FPlayerInfo info);

protected:
    AActor* _MenuActor;

    virtual void SetupInputComponent() override;

    /********************************** ACTION MAPPINGS ******************************************/
    /*************** TRIGGER MENU *************/
    void ToogleMenu();
};