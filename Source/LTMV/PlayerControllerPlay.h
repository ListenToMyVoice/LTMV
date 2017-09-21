// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayerControllerPlay.generated.h"


UCLASS()
class LTMV_API APlayerControllerPlay : public APlayerController {
    GENERATED_BODY()

public:
    APlayerControllerPlay(const FObjectInitializer& OI);

    virtual void TickActor(float DeltaTime, enum ELevelTick TickType,
                           FActorTickFunction & ThisTickFunction) override;

    virtual void BeginPlay() override;

    void AfterPossessed(bool _afterdeath);
    //This override is because CLIENT_AfterPossessed does not work in host (Client-server)
    UFUNCTION()
    void OnRep_Pawn() override;
    
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
    void CLIENT_Dead();
    UFUNCTION(Client, Reliable)
    void CLIENT_GotoState(FName NewState);

    /*************** TRIGGER MENU *************/
    void ToogleMenu();
    UFUNCTION(Client, Reliable)
    void CLIENT_ShowMenu();
    UFUNCTION(Client, Reliable)
    void CLIENT_HideMenu();

    /********* TRIGGER INVENTORY VR ***********/
    void CreateDestroyVRInventoryActor(bool IsMainMenu);
    UFUNCTION(Client, Reliable)
    void CLIENT_ToggleVRInventory();
    UFUNCTION(Client, Reliable)
    void CLIENT_CreateVRInventory();

protected:
    class UNWGameInstance* _GameInstance;

    virtual void SetupInputComponent() override;

    /********************************** ACTION MAPPINGS ******************************************/
    /******** USE ITEM LEFT *********/
    void UseLeftPressed();
    void UseLeftReleased();

    /******* USE ITEM RIGHT *********/
    void UseRightPressed();
    void UseRightReleased();

private:
    class UFMODAudioComponent* _WalkieNoiseAudioComp;
    UAudioComponent* _VoiceAudioComp;
    //UAudioComponent* _TestAudioComp;
    bool _IsListen;
    bool _ClientPossesed;
    FString _MapMainMenu;

    /* MENU INTERFACE */
    class AMenu3D* _MenuActor;
    void CreateMenuActor();

    /* VR INVENTORY INTERFACE*/
    class AVRInventory* _VRInventoryActor;

    void TickWalkie();

public:
    FORCEINLINE AVRInventory* GetVRInventory() { return _VRInventoryActor; }
};