// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NWGameInstance.h"

#include "GameFramework/PlayerController.h"
#include "PlayerControllerLobby.generated.h"


UCLASS()
class LTMV_API APlayerControllerLobby : public APlayerController {
    GENERATED_BODY()

public:
    class UFMODAudioComponent* _AudioComp;

    APlayerControllerLobby(const FObjectInitializer& OI);

    virtual void TickActor(float DeltaTime, enum ELevelTick TickType,
                           FActorTickFunction & ThisTickFunction) override;

    UFUNCTION(Client, Reliable)
    void Client_InitialSetup();
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_CallUpdate(FPlayerInfo info, bool changedStatus);


    UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Menu")
    void Client_CreateMenu(TSubclassOf<AActor> menuClass);

    virtual void ModifyVoiceAudioComponent(const FUniqueNetId& RemoteTalkerId,
                                           class UAudioComponent* AudioComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Voice")
    bool IsListen();

protected:
    AActor* _ActorWidgetMenu;

    virtual void SetupInputComponent() override;

private:
    UAudioComponent* _VoiceAudioComp;
    bool _IsListen;

    void TickWalkie();

    /********************************** ACTION MAPPINGS ******************************************/
    /***************** EXIT GAME **************/
    void ExitGame();
};
