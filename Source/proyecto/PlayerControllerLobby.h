// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayerControllerLobby.generated.h"

USTRUCT(BlueprintType, Category = "Player Info")
struct FPlayerInfo {
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    FString Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    TSubclassOf<ACharacter> CharacterClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    FString Status;
};

UCLASS()
class PROYECTO_API APlayerControllerLobby : public APlayerController {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Player Info")
    FPlayerInfo _PlayerInfo;
    UPROPERTY(Replicated)
    FPlayerInfo _OtherPlayerInfo;

    APlayerControllerLobby(const FObjectInitializer& OI);
    virtual void BeginPlay() override;

    UFUNCTION(Client, Reliable)
    void Client_InitialSetup();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_CallUpdate(FPlayerInfo info, bool changedStatus);
    UFUNCTION(Client, Reliable)
    void Client_UpdateNumberOfPlayers(int32 CurrentPlayers, int32 MaxPlayers);
    UFUNCTION(Client, Reliable)
    void Client_AddPlayerInfo(FPlayerInfo playerInfo);
    UFUNCTION(Client, Reliable)
    void Client_UpdateLobbySettings(const FString& mapName);
    UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Menu")
    void Client_CreateMenu(TSubclassOf<AActor> menuClass);

protected:
    AActor* _ActorWidgetMenu;
};
