// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "PlayerControllerPlay.h"

#include "GameModePlay.h"
#include "NWGameInstance.h"
#include "FMODAudioComponent.h"


APlayerControllerPlay::APlayerControllerPlay(const FObjectInitializer& OI) : Super(OI) {
    /* VOICE */
    _AudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Audio"));
    static ConstructorHelpers::FObjectFinder<UObject> Finder(
        TEXT("/Game/FMOD/Events/Character/Radio/CommandCustom"));

    _AudioComp->SetEvent((UFMODEvent*)(Finder.Object));
    _AudioComp->bAutoActivate = false;

    _IsListen = false;
}

void APlayerControllerPlay::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("Menu", IE_Released, this, &APlayerControllerPlay::ToogleMenu);
    InputComponent->BindAction("Exit", IE_Released, this, &APlayerControllerPlay::ExitGame);
}

void APlayerControllerPlay::BeginPlay() {
    Super::BeginPlay();

    if (IsLocalController()) {
        UNWGameInstance* gameInstance = Cast<UNWGameInstance>(GetGameInstance());
        if (gameInstance) SERVER_CallUpdate(gameInstance->_PlayerInfoSaved);
    }
}

bool APlayerControllerPlay::SERVER_CallUpdate_Validate(FPlayerInfo info) {
    return true; 
}
void APlayerControllerPlay::SERVER_CallUpdate_Implementation(FPlayerInfo info) {
    AGameModePlay* gameMode = Cast<AGameModePlay>(GetWorld()->GetAuthGameMode());
    if (gameMode) gameMode->SERVER_RespawnPlayer(this, info);
}


/*********************************************** VOICE *******************************************/
void APlayerControllerPlay::ModifyVoiceAudioComponent(const FUniqueNetId& RemoteTalkerId,
                                                      class UAudioComponent* AudioComponent) {

    if (!_VoiceAudioComp) _VoiceAudioComp = AudioComponent;

    //AudioComponent->bEnableLowPassFilter = true;
    //AudioComponent->LowPassFilterFrequency = 60000;
}

void APlayerControllerPlay::TickActor(float DeltaTime, enum ELevelTick TickType,
                                      FActorTickFunction & ThisTickFunction) {
    Super::TickActor(DeltaTime, TickType, ThisTickFunction);
    TickWalkie();
}

void APlayerControllerPlay::TickWalkie() {
    if (_VoiceAudioComp && _AudioComp) {
        if (_VoiceAudioComp->IsPlaying() && !_AudioComp->IsPlaying()) {
            ULibraryUtils::Log("Play Radio", 0, 10);
            _AudioComp->Play();
            _IsListen = true;
        }
        else if (!_VoiceAudioComp->IsPlaying() && _AudioComp->IsPlaying()) {
            ULibraryUtils::Log("Stop Radio", 2, 10);
            _AudioComp->Stop();
            _IsListen = false;
        }
    }
}

bool APlayerControllerPlay::IsListen() {
    return _IsListen;
}

/****************************************** ACTION MAPPINGS **************************************/
/*************** TRIGGER MENU *************/
void APlayerControllerPlay::ToogleMenu() {
    //if (isMenuHidden) {
    //    ULibraryUtils::SetActorEnable(_Menu);

    //    UCameraComponent* cameraComp = Cast<UCameraComponent>(
    //                                        GetPawn()->FindComponentByClass<UCameraComponent>());
    //    if (cameraComp) {
    //        FVector position = cameraComp->GetForwardVector() * 200.0f +
    //                           cameraComp->GetComponentLocation();
    //        FRotator rotation = cameraComp->GetComponentRotation();
    //        _Menu->SetActorLocationAndRotation(position,
    //                                           rotation,
    //                                           false,
    //                                           nullptr,
    //                                           ETeleportType::TeleportPhysics);
    //    }
    //}
    //else {
    //    ULibraryUtils::SetActorEnable(_Menu, false);
    //}
    //isMenuHidden = !isMenuHidden;
}

/***************** EXIT GAME **************/
void APlayerControllerPlay::ExitGame() {
    FGenericPlatformMisc::RequestExit(false);
}