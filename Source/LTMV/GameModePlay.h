// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GameModePlay.generated.h"


UCLASS()
class LTMV_API AGameModePlay : public AGameMode {
    GENERATED_BODY()

public:
    AGameModePlay(const class FObjectInitializer& OI);

    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_RespawnPlayer(APlayerController* PlayerController, FPlayerInfo info);

    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_PlayerDead(AController* PlayerController);

    virtual void InitGame(const FString & MapName, const FString & Options,
                          FString & ErrorMessage) override;

protected:
    class APlayerControllerPlay* _HostController;
    class APlayerControllerPlay* _GuestController;
};
