// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"

#include "GameFramework/PlayerController.h"
#include "PlayerControllerPlay.generated.h"


UCLASS()
class LTMV_API APlayerControllerPlay : public APlayerController {
    GENERATED_BODY()

public:
    class UFMODAudioComponent* _AudioComp;

    APlayerControllerPlay(const FObjectInitializer& OI);

    virtual void TickActor(float DeltaTime, enum ELevelTick TickType,
                           FActorTickFunction & ThisTickFunction) override;

    virtual void BeginPlay() override;

    UFUNCTION(Client, Reliable)
    void CLIENT_AfterPossessed();
    
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_CallUpdate(FPlayerInfo info);

    /*************************************** VOICE ***********************************************/
    virtual void ModifyVoiceAudioComponent(const FUniqueNetId& RemoteTalkerId,
                                           class UAudioComponent* AudioComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Voice")
    bool IsListen();

    /* Radio Delegate */
    void OnRadioPressed();
    void OnRadioReleased();

    UFUNCTION(Client, Reliable)
    void CLIENT_Dead(const FUniqueNetIdRepl NetId);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    TSubclassOf<AActor> _MenuClass;
    AActor* _MenuActor;
    bool _IsMenuHidden;

    virtual void SetupInputComponent() override;

    /********************************** ACTION MAPPINGS ******************************************/
    /*************** TRIGGER MENU *************/
    void ToogleMenu();

private:
    UAudioComponent* _VoiceAudioComp;
    bool _IsListen;

    void TickWalkie();

    /***************** EXIT GAME **************/
    void ExitGame();
};