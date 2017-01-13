// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CharacterController.generated.h"

UCLASS()
class PROYECTO_API ACharacterController : public APlayerController {
    GENERATED_BODY()

public:

    ACharacterController();
    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_TakeLeft();

    UFUNCTION(NetMulticast, Reliable)
    void TakeLeft();

protected:
    virtual void SetupInputComponent() override;
};