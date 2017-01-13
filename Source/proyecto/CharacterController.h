// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CharacterController.generated.h"

UCLASS()
class PROYECTO_API ACharacterController : public APlayerController {
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
};
