// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "PlayerControllerLobby.h"

#include "GameModeLobby.h"
#include "FMODAudioComponent.h"


APlayerControllerLobby::APlayerControllerLobby(const FObjectInitializer& OI) : Super(OI) {
    _AudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Audio"));
    //static ConstructorHelpers::FObjectFinder<UObject> Finder(
    //    TEXT("/Game/FMOD/Desktop/Events/Character/Radio/CommandCustom"));

    //_AudioComp->SetEvent((UFMODEvent*)(Finder.Object));
    _AudioComp->bAutoActivate = false;

    _IsListen = false;
}

void APlayerControllerLobby::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("Exit", IE_Released, this, &APlayerControllerLobby::ExitGame);
}

void APlayerControllerLobby::Client_InitialSetup_Implementation() {
    UNWGameInstance* gameInstance = Cast<UNWGameInstance>(GetGameInstance());
    if (gameInstance) SERVER_CallUpdate(gameInstance->_PlayerInfoSaved, false);
}

bool APlayerControllerLobby::SERVER_CallUpdate_Validate(FPlayerInfo info,
                                                        bool changedStatus) { return true; }
void APlayerControllerLobby::SERVER_CallUpdate_Implementation(FPlayerInfo info,
                                                              bool changedStatus) {
    AGameModeLobby* gameMode = Cast<AGameModeLobby>(GetWorld()->GetAuthGameMode());
    if (gameMode) gameMode->SERVER_SwapCharacter(this, info, changedStatus);
}

void APlayerControllerLobby::Client_CreateMenu_Implementation(TSubclassOf<AActor> menuClass) {
    APawn* pawn = GetPawn();
    if (pawn) {
        UCameraComponent* cameraComp = Cast<UCameraComponent>(pawn->FindComponentByClass<UCameraComponent>());
        if (cameraComp) {
            if (_ActorWidgetMenu) _ActorWidgetMenu->Destroy();
            FVector position = cameraComp->GetComponentLocation();
            FRotator rotation = cameraComp->GetComponentRotation();

            _ActorWidgetMenu = GetWorld()->SpawnActor(menuClass, &position, &rotation);
        }
    }
}

void APlayerControllerLobby::ExitGame() {
    FGenericPlatformMisc::RequestExit(false);
}

void APlayerControllerLobby::ModifyVoiceAudioComponent(const FUniqueNetId& RemoteTalkerId,
                                                       class UAudioComponent* AudioComponent) {
    
    if (!_VoiceAudioComp) _VoiceAudioComp = AudioComponent;
    
    //AudioComponent->bEnableLowPassFilter = true;
    //AudioComponent->LowPassFilterFrequency = 60000;
}

void APlayerControllerLobby::TickActor(float DeltaTime, enum ELevelTick TickType,
                                       FActorTickFunction & ThisTickFunction) {
    Super::TickActor(DeltaTime, TickType, ThisTickFunction);
    TickWalkie();
}

void APlayerControllerLobby::TickWalkie() {
    if(_VoiceAudioComp && _AudioComp) {
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

bool APlayerControllerLobby::IsListen() {
    return _IsListen;
}