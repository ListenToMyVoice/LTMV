// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerControllerLobby.h"

#include "GameFramework/GameMode.h"
#include "GameModeLobby.generated.h"


UCLASS()
class PROYECTO_API AGameModeLobby : public AGameMode {
    GENERATED_BODY()

public:
    AGameModeLobby(const class FObjectInitializer& OI);
    virtual void BeginPlay() override;

    virtual void PostLogin(APlayerController * NewPlayer) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_SwapCharacter(APlayerController* PlayerController,
                              TSubclassOf<ACharacter> CharacterClass, bool ChangeStatus);
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_UpdateEveryOne();

protected:
    UPROPERTY(Replicated)
    FName _ServerName;
    UPROPERTY(Replicated)
    FName _MapNameGM;

    UPROPERTY(Replicated)
    int32 _MaxPlayers;
    UPROPERTY(Replicated)
    int32 _CurrentPlayers;

    UPROPERTY(Replicated)
    APlayerController* _PlayerController_1;
    UPROPERTY(Replicated)
    APlayerController* _PlayerController_2;
    UPROPERTY(Replicated)
    APlayerStart* _SpawnPoint_1;
    UPROPERTY(Replicated)
    APlayerStart* _SpawnPoint_2;
    //UPROPERTY(Replicated)
    //FPlayerInfo _PlayerInfo_1;
    //UPROPERTY(Replicated)
    //FPlayerInfo _PlayerInfo_2;

    void FindSpawnPoints();
};
