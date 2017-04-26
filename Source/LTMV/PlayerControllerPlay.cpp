// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "PlayerControllerPlay.h"

#include "GameModePlay.h"
#include "NWGameInstance.h"
#include "FMODAudioComponent.h"
#include "PlayerCharacter.h"


APlayerControllerPlay::APlayerControllerPlay(const FObjectInitializer& OI) : Super(OI) {
    /* VOICE */
    _WalkieNoiseAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Audio"));
    static ConstructorHelpers::FObjectFinder<UObject> Finder(
        TEXT("/Game/FMOD/Events/Radio/Interferencia_radio"));
    _WalkieNoiseAudioComp->SetEvent((UFMODEvent*)(Finder.Object));
    _WalkieNoiseAudioComp->bAutoActivate = false;
    _IsListen = false;

    /* MENU */
    _IsMenuHidden = true;
    static ConstructorHelpers::FClassFinder<AActor> MenuClassFinder(TEXT(
        "/Game/BluePrints/HUD/MenuPlayActor_BP"));
    _MenuClass = MenuClassFinder.Class;

    _ClientPossesed = false;


    //static ConstructorHelpers::FObjectFinder<USoundWave> SoundFinder(
    //    TEXT("/Game/Maps/Alex/04_-_Black_Night_Of_Magic"));
    //_TestAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Voice Comp"));
    //_TestAudioComp->bAutoActivate = false;
    //_TestAudioComp->SetSound(SoundFinder.Object);
}

void APlayerControllerPlay::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("Menu", IE_Released, this, &APlayerControllerPlay::ToogleMenu);

    /* USE ITEM */
    InputComponent->BindAction("ClickLeft", IE_Pressed, this, &APlayerControllerPlay::UseLeftPressed);
    InputComponent->BindAction("ClickLeft", IE_Released, this, &APlayerControllerPlay::UseLeftReleased);
    InputComponent->BindAction("ClickRight", IE_Pressed, this, &APlayerControllerPlay::UseRightPressed);
    InputComponent->BindAction("ClickRight", IE_Released, this, &APlayerControllerPlay::UseRightReleased);
}

void APlayerControllerPlay::BeginPlay() {
    Super::BeginPlay();

    if (IsLocalController()) {
        UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetGameInstance());
        if (GameInstance) SERVER_CallUpdate(GameInstance->_PlayerInfoSaved);
    }
}

bool APlayerControllerPlay::SERVER_CallUpdate_Validate(FPlayerInfo info) {
    return true;
}
void APlayerControllerPlay::SERVER_CallUpdate_Implementation(FPlayerInfo info) {
    AGameModePlay* gameMode = Cast<AGameModePlay>(GetWorld()->GetAuthGameMode());
    if (gameMode) gameMode->SERVER_RespawnPlayer(this, info);
}

void APlayerControllerPlay::AfterPossessed() {
    /* CLIENT-SERVER EXCEPTION */
    if (!_ClientPossesed) {
        UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetGameInstance());
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
        if (!GameInstance || !PlayerCharacter) return;

        if (PlayerCharacter->IsA(GameInstance->_PlayerInfoSaved.CharacterClass)) {
            PlayerCharacter->_OnRadioPressedDelegate.BindUObject(this, &APlayerControllerPlay::OnRadioPressed);
            PlayerCharacter->_OnRadioReleasedDelegate.BindUObject(this, &APlayerControllerPlay::OnRadioReleased);
            PlayerCharacter->AfterPossessed(true);
            _ClientPossesed = true;
        }
    }
}

void APlayerControllerPlay::OnRep_Pawn() {
    Super::OnRep_Pawn();
    /* CLIENT-SERVER EXCEPTION  */
    AfterPossessed();
}

/*********************************************** VOICE *******************************************/
void APlayerControllerPlay::ModifyVoiceAudioComponent(const FUniqueNetId& RemoteTalkerId,
                                                      class UAudioComponent* AudioComponent) {

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    bool FullVolume = false;
    if (PlayerCharacter) {
        FullVolume = PlayerCharacter->IsWalkieInHand();

        if (!_VoiceAudioComp) {
            AActor* WalkieActor = PlayerCharacter->GetWalkieActor();
            if (WalkieActor) {
                UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(
                    WalkieActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

                if (MeshComponent) {
                    AudioComponent->AttachToComponent(MeshComponent,
                                                      FAttachmentTransformRules::KeepRelativeTransform);
                    AudioComponent->bOverrideAttenuation = true;
                    _VoiceAudioComp = AudioComponent;

                    _WalkieNoiseAudioComp->AttachToComponent(MeshComponent,
                                                             FAttachmentTransformRules::KeepRelativeTransform);
                    _WalkieNoiseAudioComp->bOverrideAttenuation = true;
                    ULibraryUtils::Log("Setup Voice");

                    //_TestAudioComp->AttachToComponent(MeshComponent,
                    //                                  FAttachmentTransformRules::KeepRelativeTransform);
                    //_TestAudioComp->bOverrideAttenuation = true;
                    //_TestAudioComp->Play();
                }
            }
        }

        if (FullVolume) {
            _VoiceAudioComp->SetVolumeMultiplier(1.0);
            _WalkieNoiseAudioComp->SetVolume(1.0);
            //_TestAudioComp->SetVolumeMultiplier(1.0);
            ULibraryUtils::Log("VOLUME: 1.0");
        }
        else {
            _VoiceAudioComp->SetVolumeMultiplier(0.05);
            _WalkieNoiseAudioComp->SetVolume(0.05);
            //_TestAudioComp->SetVolumeMultiplier(0.05);
            ULibraryUtils::Log("VOLUME: 0.05");
        }
    }
}

void APlayerControllerPlay::TickActor(float DeltaTime, enum ELevelTick TickType,
                                      FActorTickFunction & ThisTickFunction) {
    Super::TickActor(DeltaTime, TickType, ThisTickFunction);
    TickWalkie();
}

void APlayerControllerPlay::TickWalkie() {
    if (_VoiceAudioComp && _WalkieNoiseAudioComp) {
        if (_VoiceAudioComp->IsPlaying() && !_WalkieNoiseAudioComp->IsPlaying()) {
            _WalkieNoiseAudioComp->Play();
            _IsListen = true;
        }
        else if (!_VoiceAudioComp->IsPlaying() && _WalkieNoiseAudioComp->IsPlaying()) {
            _WalkieNoiseAudioComp->Stop();
            _IsListen = false;
        }
    }
}

bool APlayerControllerPlay::IsListen() {
    return _IsListen;
}

/****************************************** ACTION MAPPINGS **************************************/

/******** USE ITEM LEFT *********/
void APlayerControllerPlay::UseLeftPressed() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) PlayerCharacter->UseLeftPressed(_IsMenuHidden);
}

void APlayerControllerPlay::UseLeftReleased() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) PlayerCharacter->UseLeftReleased(_IsMenuHidden);
}

/******* USE ITEM RIGHT *********/
void APlayerControllerPlay::UseRightPressed() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) PlayerCharacter->UseRightPressed(_IsMenuHidden);
}

void APlayerControllerPlay::UseRightReleased() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) PlayerCharacter->UseRightReleased(_IsMenuHidden);
}

/*************** TRIGGER MENU *************/
void APlayerControllerPlay::ToogleMenu() {
    if (GetPawnOrSpectator()) {
        if (_IsMenuHidden) {
            UCameraComponent* cameraComp = Cast<UCameraComponent>(GetPawnOrSpectator()->FindComponentByClass<UCameraComponent>());
            if (cameraComp) {
                FVector position = cameraComp->GetComponentLocation();
                FRotator rotation = cameraComp->GetComponentRotation();

                if (_MenuActor) {
                    ULibraryUtils::SetActorEnable(_MenuActor);
                    _MenuActor->SetActorLocationAndRotation(position,
                                                            rotation,
                                                            false,
                                                            nullptr,
                                                            ETeleportType::TeleportPhysics);
                }
                else {
                    _MenuActor = GetWorld()->SpawnActor(_MenuClass, &position, &rotation);
                }
            }
        }
        else {
            ULibraryUtils::SetActorEnable(_MenuActor, false);
        }
        _IsMenuHidden = !_IsMenuHidden;
    }
}


/***************** EXIT GAME **************/
void APlayerControllerPlay::ExitGame() {
    FGenericPlatformMisc::RequestExit(false);
}

/*********************************************** DELEGATES ***************************************/
void APlayerControllerPlay::OnRadioPressed() {
    StartTalking();

    ULibraryUtils::Log(FString::Printf(TEXT("I AM: %s"),
                                       *PlayerState->UniqueId.ToDebugString()), 3, 60);

    for (APlayerState* OtherPlayerState : GetWorld()->GetGameState()->PlayerArray) {
        if (PlayerState->UniqueId != OtherPlayerState->UniqueId) {
            ClientMutePlayer(OtherPlayerState->UniqueId);
            ULibraryUtils::Log(FString::Printf(TEXT("MUTE: %s"),
                                               *OtherPlayerState->UniqueId.ToDebugString()), 2, 60);
        }
    }
}

void APlayerControllerPlay::OnRadioReleased() {
    StopTalking();

    for (APlayerState* OtherPlayerState : GetWorld()->GetGameState()->PlayerArray) {
        if (PlayerState->UniqueId != OtherPlayerState->UniqueId) {
            ClientUnmutePlayer(OtherPlayerState->UniqueId);
            ULibraryUtils::Log(FString::Printf(TEXT("UNMUTE: %s"),
                                               *OtherPlayerState->UniqueId.ToDebugString()), 0, 60);
        }
    }
}

/******************************************** GAME FLOW ******************************************/
void APlayerControllerPlay::CLIENT_Dead_Implementation(const FUniqueNetIdRepl NetId) {
    ToogleMenu();
}

void APlayerControllerPlay::CLIENT_GotoState_Implementation(FName NewState) {
    if (GetPawn()) {// CLIENT-SERVER EXCEPTION
        FVector Location = GetPawn()->GetActorLocation();
        ClientGotoState(NewState);
        GetSpectatorPawn()->SetActorLocation(Location);
    }
}