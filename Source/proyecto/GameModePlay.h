// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GameModePlay.generated.h"


UCLASS()
class PROYECTO_API AGameModePlay : public AGameMode {
    GENERATED_BODY()

public:
    AGameModePlay(const class FObjectInitializer& OI);

    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_RespawnPlayer(APlayerController* PlayerController,
                              TSubclassOf<ACharacter> CharacterClass);

protected:
    UPROPERTY(Replicated)
    int32 _CurrentPlayers;
};
