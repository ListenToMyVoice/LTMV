// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "VRPlayerController.generated.h"


UCLASS()
class PROYECTO_API AVRPlayerController : public APlayerController {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    TSubclassOf<AActor> _MenuClass;

    AVRPlayerController(const FObjectInitializer& OI);
    virtual void BeginPlay() override;

protected:
    virtual void SetupInputComponent() override;

    /********************************** ACTION MAPPINGS ******************************************/
    /*************** TRIGGER MENU *************/
    void ToogleMenu();
};
