// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"

#include "GameFramework/PlayerController.h"
#include "PlayerControllerPlay.generated.h"


UCLASS()
class LTMV_API APlayerControllerPlay : public APlayerController {
    GENERATED_BODY()

public:
    class UFMODAudioComponent* _WalkieNoiseAudioComp;

    APlayerControllerPlay(const FObjectInitializer& OI);

    virtual void TickActor(float DeltaTime, enum ELevelTick TickType,
                           FActorTickFunction & ThisTickFunction) override;

    virtual void BeginPlay() override;

    void AfterPossessed();
    //This override is because CLIENT_AfterPossessed does not work in host (Client-server)
    UFUNCTION()
    virtual void OnRep_Pawn() override;
    
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_CallUpdate(FPlayerInfo info);

    /***********************************INVENTORY WIDGET******************************************/
    UInventoryWidget* _InventoryWidget;

    void SetupInventoryWidget(APlayerCharacter* PlayerCharacter);

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
    UFUNCTION(Client, Reliable)
    void CLIENT_GotoState(FName NewState);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    TSubclassOf<AActor> _MenuClass;
    AActor* _MenuActor;
    bool _IsMenuHidden;
    bool _IsInventoryHidden;

    virtual void SetupInputComponent() override;

    /********************************** ACTION MAPPINGS ******************************************/
    /******** USE ITEM LEFT *********/
    void UseLeftPressed();
    void UseLeftReleased();

    /******* USE ITEM RIGHT *********/
    void UseRightPressed();
    void UseRightReleased();

    /*************** TRIGGER MENU *************/
    void ToogleMenu();

    /**************** TRIGGER INVENTORY *************/
    void ToggleInventory();

private:
    UAudioComponent* _VoiceAudioComp;
    bool _IsListen;
    bool _ClientPossesed;

    void TickWalkie();

    /***************** EXIT GAME **************/
    void ExitGame();
};