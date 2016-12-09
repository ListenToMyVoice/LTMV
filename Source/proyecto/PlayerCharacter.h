// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PROYECTO_API APlayerCharacter : public ACharacter {
    GENERATED_BODY()

public:
    APlayerCharacter();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

protected:

    void MoveForward(float Value);

    void MoveRight(float Value);

    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
};
