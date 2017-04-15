// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
    UFUNCTION(Server, Reliable, WithValidation)
    void SERVER_CallUpdate(FPlayerInfo info);

    /***********************************INVENTORY WIDGET******************************************/
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
    TSubclassOf<class UInventoryWidget> InventoryUIClass;

    UInventoryWidget* _inventoryHUD;

    class APlayerCharacter* _PlayerCharacter;

    void SetupInventoryWidget(UInventoryWidget* InventoryWidget);

    virtual void Possess(APawn *InPawn) override;

    /*************************************** VOICE ***********************************************/
    virtual void ModifyVoiceAudioComponent(const FUniqueNetId& RemoteTalkerId,
                                           class UAudioComponent* AudioComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Voice")
    bool IsListen();

    UFUNCTION(Client, Reliable)
    void CLIENT_Dead(const uint32 NetId);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    TSubclassOf<AActor> _MenuClass;
    AActor* _MenuActor;
    bool _IsMenuHidden;
    bool _IsInventoryHidden;

    virtual void SetupInputComponent() override;

    /********************************** ACTION MAPPINGS ******************************************/
    /*************** TRIGGER MENU *************/
    void ToogleMenu();

    /*************** PUSH TO TALK *************/
    void PushTalk();
    void ReleaseTalk();

    /**************** TRIGGER INVENTORY *************/
    void ToggleInventory();

private:
    UAudioComponent* _VoiceAudioComp;
    bool _IsListen;

    void TickWalkie();

    /***************** EXIT GAME **************/
    void ExitGame();
};