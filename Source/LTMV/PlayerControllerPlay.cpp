// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "PlayerControllerPlay.h"

#include "GameModePlay.h"
#include "NWGameInstance.h"
#include "FMODAudioComponent.h"
#include "PlayerCharacter.h"
#include "PlayerSpectator.h"
#include "VRInventory.h"
#include "VRCharacter.h"
#include "Menu3D.h"
#include "Walkie.h"


APlayerControllerPlay::APlayerControllerPlay(const FObjectInitializer& OI) : Super(OI) {
    GConfig->GetString(
        TEXT("/Script/EngineSettings.GameMapsSettings"),
        TEXT("GameDefaultMap"),
        _MapMainMenu,
        GEngineIni
    );

    /* VOICE */
    _WalkieNoiseAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Audio"));
    //static ConstructorHelpers::FObjectFinder<UObject> Finder(
    //    TEXT("/Game/FMOD/Desktop/Events/Radio/Interferencia_radio"));
    //_WalkieNoiseAudioComp->SetEvent((UFMODEvent*)(Finder.Object));
    _WalkieNoiseAudioComp->bAutoActivate = false;
    _IsListen = false;
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

    InputComponent->BindAction("ToggleVRInventory", IE_Released, this, &APlayerControllerPlay::CLIENT_ToggleVRInventory);

    /* USE ITEM */
    InputComponent->BindAction("ClickLeft", IE_Pressed, this, &APlayerControllerPlay::UseLeftPressed);
    InputComponent->BindAction("ClickLeft", IE_Released, this, &APlayerControllerPlay::UseLeftReleased);
    InputComponent->BindAction("ClickRight", IE_Pressed, this, &APlayerControllerPlay::UseRightPressed);
    InputComponent->BindAction("ClickRight", IE_Released, this, &APlayerControllerPlay::UseRightReleased);
}

void APlayerControllerPlay::BeginPlay() {
    Super::BeginPlay();

    if (IsLocalController()) {
        _GameInstance = Cast<UNWGameInstance>(GetGameInstance());
        if (_GameInstance) {
            SERVER_CallUpdate(_GameInstance->_PlayerInfoSaved);
        }
        if (_GameInstance->_IsVRMode) CLIENT_CreateVRInventory();
    }
}

bool APlayerControllerPlay::SERVER_CallUpdate_Validate(FPlayerInfo info) {
    return true;
}
void APlayerControllerPlay::SERVER_CallUpdate_Implementation(FPlayerInfo info) {
    AGameModePlay* gameMode = Cast<AGameModePlay>(GetWorld()->GetAuthGameMode());
    if (gameMode) gameMode->SERVER_RespawnPlayer(this, info);
}

void APlayerControllerPlay::AfterPossessed(bool _afterdeath) {
    /* CLIENT-SERVER EXCEPTION */
    if (!_ClientPossesed) {
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
        if (!_GameInstance || !PlayerCharacter) return;

        if (PlayerCharacter->IsA(_GameInstance->_PlayerInfoSaved.CharacterClass)) {
            PlayerCharacter->_OnRadioPressedDelegate.BindUObject(this, &APlayerControllerPlay::OnRadioPressed);
            PlayerCharacter->_OnRadioReleasedDelegate.BindUObject(this, &APlayerControllerPlay::OnRadioReleased);
            if (_afterdeath)	PlayerCharacter->AfterPossessed(true,true);
			else				PlayerCharacter->AfterPossessed(true, false);
            _ClientPossesed = true;
        }
    }
	if (_afterdeath) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
		if (!_GameInstance || !PlayerCharacter) return;

		if (PlayerCharacter->IsA(_GameInstance->_PlayerInfoSaved.CharacterClass)) {
			PlayerCharacter->_OnRadioPressedDelegate.BindUObject(this, &APlayerControllerPlay::OnRadioPressed);
			PlayerCharacter->_OnRadioReleasedDelegate.BindUObject(this, &APlayerControllerPlay::OnRadioReleased);
			PlayerCharacter->AfterPossessed(true, true);
			_ClientPossesed = true;
		}
	}
}

void APlayerControllerPlay::OnRep_Pawn() {
    Super::OnRep_Pawn();
    /* CLIENT-SERVER EXCEPTION  */
    AfterPossessed(false);
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
                UFMODEvent* NoiseEvent = PlayerCharacter->GetWalkieEvent();
                UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(
                    WalkieActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

                if (MeshComponent && NoiseEvent) {
                    AudioComponent->AttachToComponent(MeshComponent,
                                                      FAttachmentTransformRules::KeepRelativeTransform);
                    AudioComponent->bOverrideAttenuation = true;
                    _VoiceAudioComp = AudioComponent;

                    _WalkieNoiseAudioComp->SetEvent(NoiseEvent);
                    _WalkieNoiseAudioComp->AttachToComponent(MeshComponent,
                                                             FAttachmentTransformRules::KeepRelativeTransform);
                    _WalkieNoiseAudioComp->bOverrideAttenuation = true;
                    //ULibraryUtils::Log("Setup Voice");


                    //_TestAudioComp->AttachToComponent(MeshComponent,
                    //                                  FAttachmentTransformRules::KeepRelativeTransform);
                    //_TestAudioComp->bOverrideAttenuation = true;
                    //_TestAudioComp->Play();
                }
            }
        }

        if (FullVolume) {
            if (_VoiceAudioComp) {
                _VoiceAudioComp->SetVolumeMultiplier(2.0);
                _WalkieNoiseAudioComp->SetVolume(2.0);
                //_TestAudioComp->SetVolumeMultiplier(1.0);
                //ULibraryUtils::Log("VOLUME: 1.0");
            }
        }
        else {
            if (_VoiceAudioComp) {
                _VoiceAudioComp->SetVolumeMultiplier(0.5);
                _WalkieNoiseAudioComp->SetVolume(0.5);
                //_TestAudioComp->SetVolumeMultiplier(0.05);
                //ULibraryUtils::Log("VOLUME: 0.05");
            }
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

		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
		AActor* WalkieActor = PlayerCharacter->GetWalkieActor();

        if (_VoiceAudioComp->IsPlaying() && !_WalkieNoiseAudioComp->IsPlaying()) {
            _WalkieNoiseAudioComp->Play();
            _IsListen = true;

			UWalkie* WalkieComp = Cast<UWalkie>(WalkieActor->GetComponentByClass(UWalkie::StaticClass()));
			if (WalkieComp) {
				////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("WALKIE: TOGGLE OTHER LIGHT ")));
				WalkieComp->ToggleOtherLight(true);
				WalkieComp->SetMute(true);
			}
        }
        else if (!_VoiceAudioComp->IsPlaying() && _WalkieNoiseAudioComp->IsPlaying()) {
            _WalkieNoiseAudioComp->Stop();
            _IsListen = false;

			UWalkie* WalkieComp = Cast<UWalkie>(WalkieActor->GetComponentByClass(UWalkie::StaticClass()));
			if (WalkieComp) {
				WalkieComp->ToggleOtherLight(false);
				WalkieComp->SetMute(false);
			}
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
    if (PlayerCharacter) {
        bool IsMenuHidden = _MenuActor ? _MenuActor->_IsMenuHidden : true;
        PlayerCharacter->UseLeftPressed(IsMenuHidden);
    }
    else {
        APlayerSpectator* PlayerSpectator = Cast<APlayerSpectator>(GetSpectatorPawn());
        if (PlayerSpectator) PlayerSpectator->ClickLeftPressed();
    }
}

void APlayerControllerPlay::UseLeftReleased() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) {
        bool IsMenuHidden = _MenuActor ? _MenuActor->_IsMenuHidden : true;
        PlayerCharacter->UseLeftReleased(IsMenuHidden);
    }
    else {
        APlayerSpectator* PlayerSpectator = Cast<APlayerSpectator>(GetSpectatorPawn());
        if (PlayerSpectator) PlayerSpectator->ClickLeftReleased();
    }
}

/******* USE ITEM RIGHT *********/
void APlayerControllerPlay::UseRightPressed() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) {
        bool IsMenuHidden = _MenuActor ? _MenuActor->_IsMenuHidden : true;
        PlayerCharacter->UseRightPressed(IsMenuHidden);
    }
    else {
        APlayerSpectator* PlayerSpectator = Cast<APlayerSpectator>(GetSpectatorPawn());
        if (PlayerSpectator) PlayerSpectator->ClickLeftPressed();
    }
}

void APlayerControllerPlay::UseRightReleased() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) {
        bool IsMenuHidden = _MenuActor ? _MenuActor->_IsMenuHidden : true;
        PlayerCharacter->UseRightReleased(IsMenuHidden);
    }
    else {
        APlayerSpectator* PlayerSpectator = Cast<APlayerSpectator>(GetSpectatorPawn());
        if (PlayerSpectator) PlayerSpectator->ClickLeftReleased();
    }
}

/*************** TRIGGER MENU *************/
void APlayerControllerPlay::ToogleMenu() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) {
        /* MENU INTERFACE */
        if(!_MenuActor) CreateMenuActor();

        UCameraComponent* CameraComp = Cast<UCameraComponent>(PlayerCharacter->
                                                              FindComponentByClass<UCameraComponent>());
        if (CameraComp) {
            FVector Location = CameraComp->GetComponentLocation() +
                              (CameraComp->GetForwardVector().GetSafeNormal() * 200);

            _MenuActor->ToogleMenu(Location,
                                   CameraComp->GetComponentRotation());
            PlayerCharacter->ToggleMenuInteraction(!_MenuActor->_IsMenuHidden);
        }
    }
}

void APlayerControllerPlay::CreateMenuActor() {
    UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetGameInstance());
    if (GameInstance) _MenuActor = GameInstance->CreateMenuPlay();
}

void APlayerControllerPlay::CLIENT_ShowMenu_Implementation() {
    if (!_MenuActor) CreateMenuActor();
    if (_MenuActor->_IsMenuHidden) {
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
        if (PlayerCharacter) {
            UCameraComponent* CameraComp = Cast<UCameraComponent>(PlayerCharacter->
                                                                  FindComponentByClass<UCameraComponent>());
            if (CameraComp) {
                FVector Location = CameraComp->GetComponentLocation() +
                                  (CameraComp->GetForwardVector().GetSafeNormal() * 200);

                _MenuActor->ToogleMenu(Location,
                                       CameraComp->GetComponentRotation());
                PlayerCharacter->ToggleMenuInteraction(true);
            }
        }
    }
}

void APlayerControllerPlay::CLIENT_HideMenu_Implementation() {
    bool  IsMenuHidden = _MenuActor ? _MenuActor->_IsMenuHidden : true;
    if (!IsMenuHidden) {
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
        if (PlayerCharacter) {
            UCameraComponent* CameraComp = Cast<UCameraComponent>(PlayerCharacter->
                                                                  FindComponentByClass<UCameraComponent>());
            if (CameraComp) {
                _MenuActor->ToogleMenu(CameraComp->GetComponentLocation(),
                                       CameraComp->GetComponentRotation());
                PlayerCharacter->ToggleMenuInteraction(false);
            }
        }
    }
}

/******************************** TRIGGER VR INVENTORY *******************************************/
void APlayerControllerPlay::CLIENT_CreateVRInventory_Implementation() {
    UNWGameInstance* GI = Cast<UNWGameInstance>(GetGameInstance());
    if (GI && GI->_IsVRMode) {
        if (_MapMainMenu.Contains(GetWorld()->GetMapName())) CreateDestroyVRInventoryActor(true);
        else CreateDestroyVRInventoryActor(false);
    }
}

void APlayerControllerPlay::CLIENT_ToggleVRInventory_Implementation() {
    if (!_MapMainMenu.Contains(GetWorld()->GetMapName())) {
        /* INVENTORY INTERFACE */
        if (!_VRInventoryActor) CLIENT_CreateVRInventory();

        // Show inventory
        if (GetPawnOrSpectator() && _VRInventoryActor->bIsVRInventoryHidden) {
            UCameraComponent* CameraComp = Cast<UCameraComponent>(GetPawnOrSpectator()->
                FindComponentByClass<UCameraComponent>());
            if (CameraComp) {
                FVector Location = CameraComp->GetComponentLocation() +
                    (CameraComp->GetForwardVector().GetSafeNormal() * 200);

                _VRInventoryActor->ToggleVRInventory(Location,
                                                     CameraComp->GetComponentRotation().GetInverse());

                AVRCharacter* VRPlayer = Cast<AVRCharacter>(GetPawn());
                if (VRPlayer) {
                    VRPlayer->ToggleInventoryInteraction(!_VRInventoryActor->bIsVRInventoryHidden);
                }
            }
        }
        // Hide inventory
        else if (GetPawnOrSpectator() && !_VRInventoryActor->bIsVRInventoryHidden) {
            UCameraComponent* CameraComp = Cast<UCameraComponent>(GetPawnOrSpectator()->
                FindComponentByClass<UCameraComponent>());
            if (CameraComp) {
                FVector Location = CameraComp->GetComponentLocation() +
                    (CameraComp->GetForwardVector().GetSafeNormal() * 200);

                _VRInventoryActor->ToggleVRInventory(Location,
                                                     CameraComp->GetComponentRotation().GetInverse());

                AVRCharacter* VRPlayer = Cast<AVRCharacter>(GetPawn());
                if (VRPlayer) {
                    VRPlayer->ToggleInventoryInteraction(!_VRInventoryActor->bIsVRInventoryHidden);
                }
            }
        }
    }
}

void APlayerControllerPlay::CreateDestroyVRInventoryActor(bool IsMainMenu) {
    UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetGameInstance());
    if (GameInstance) {
        if (!IsMainMenu) {
            _VRInventoryActor = GameInstance->CreateVRInventory();
        }
        else {
            _VRInventoryActor->Destroy();
        }
    }
}

/*********************************************** DELEGATES ***************************************/
void APlayerControllerPlay::OnRadioPressed() {
    StartTalking();


	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	AActor* WalkieActor = PlayerCharacter->GetWalkieActor();
	UWalkie* WalkieComp = Cast<UWalkie>(WalkieActor->GetComponentByClass(UWalkie::StaticClass()));
	if (WalkieComp) {
		WalkieComp->ToggleLight(true);
	}

    //ULibraryUtils::Log(FString::Printf(TEXT("I AM: %s")

    for (APlayerState* OtherPlayerState : GetWorld()->GetGameState()->PlayerArray) {
        if (PlayerState->UniqueId != OtherPlayerState->UniqueId) {
            ClientMutePlayer(OtherPlayerState->UniqueId);
            //ULibraryUtils::Log(FString::Printf(TEXT("MUTE: %s"),
        }
    }
}

void APlayerControllerPlay::OnRadioReleased() {
    StopTalking();

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	AActor* WalkieActor = PlayerCharacter->GetWalkieActor();
	UWalkie* WalkieComp = Cast<UWalkie>(WalkieActor->GetComponentByClass(UWalkie::StaticClass()));
	if (WalkieComp) {
		WalkieComp->ToggleLight(false);
	}
    for (APlayerState* OtherPlayerState : GetWorld()->GetGameState()->PlayerArray) {
        if (PlayerState->UniqueId != OtherPlayerState->UniqueId) {
            ClientUnmutePlayer(OtherPlayerState->UniqueId);
            //ULibraryUtils::Log(FString::Printf(TEXT("UNMUTE: %s"),
        }
    }
}

/******************************************** GAME FLOW ******************************************/
void APlayerControllerPlay::CLIENT_Dead_Implementation() {
    bool  IsMenuHidden = _MenuActor ? _MenuActor->_IsMenuHidden : true;
    APlayerSpectator* PlayerSpectator = Cast<APlayerSpectator>(GetSpectatorPawn());
    if (IsMenuHidden && PlayerSpectator) {
        /* MENU INTERFACE */
        if (!_MenuActor) CreateMenuActor();

        UCameraComponent* CameraComp = Cast<UCameraComponent>(PlayerSpectator->
                                                              FindComponentByClass<UCameraComponent>());
        if (CameraComp) {
            FVector Location = CameraComp->GetComponentLocation() +
                (CameraComp->GetForwardVector().GetSafeNormal() * 200);

            _MenuActor->ToogleMenu(Location,
                                   CameraComp->GetComponentRotation());
            PlayerSpectator->ToggleMenuInteraction(true);
        }
    }
}

void APlayerControllerPlay::CLIENT_GotoState_Implementation(FName NewState) {
    if (GetPawn()) {// CLIENT-SERVER EXCEPTION
        FVector Location = GetPawn()->GetActorLocation();
        ClientGotoState(NewState);
        GetSpectatorPawn()->SetActorLocation(Location);
        CLIENT_Dead();
    }
}