// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "PlayerControllerPlay.h"

#include "GameModePlay.h"
#include "NWGameInstance.h"
#include "FMODAudioComponent.h"
#include "PlayerCharacter.h"
#include "InventoryWidget.h"


APlayerControllerPlay::APlayerControllerPlay(const FObjectInitializer& OI) : Super(OI) {
    /* VOICE */
    _WalkieNoiseAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Audio"));
    static ConstructorHelpers::FObjectFinder<UObject> Finder(
        TEXT("/Game/FMOD/Desktop/Events/Radio/Interferencia_radio"));
    _WalkieNoiseAudioComp->SetEvent((UFMODEvent*)(Finder.Object));
    _WalkieNoiseAudioComp->bAutoActivate = false;
    _IsListen = false;

    /* MENU */
    _IsMenuHidden = true;
    static ConstructorHelpers::FClassFinder<AActor> MenuClassFinder(TEXT(
        "/Game/BluePrints/HUD/MenuPlayActor_BP"));
    _MenuClass = MenuClassFinder.Class;

    _DelegatesBinded = false;
}

void APlayerControllerPlay::SetupInputComponent() {
    Super::SetupInputComponent();
    InputComponent->BindAction("Menu", IE_Released, this, &APlayerControllerPlay::ToogleMenu);
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &APlayerControllerPlay::ToggleInventory);

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

/***************************************INVENTORY WIDGET********************************************/
void APlayerControllerPlay::SetupInventoryWidget() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter && IsLocalPlayerController()) {
        _InventoryWidget = CreateWidget<UInventoryWidget>(this,
                                                          PlayerCharacter->InventoryUIClass);
        if (_InventoryWidget) {
            _InventoryWidget->AddToViewport(); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
            _InventoryWidget->SetVisibility(ESlateVisibility::Hidden); // Set it to hidden so its not open on spawn.
            _IsInventoryHidden = true;
        }
    }
}

void APlayerControllerPlay::CLIENT_AfterPossessed_Implementation() {
    /* CLIENT-SERVER EXCEPTION  */
    if (!_DelegatesBinded) BindDelegates();
    SetupInventoryWidget();
}

void APlayerControllerPlay::OnRep_Pawn() {
    Super::OnRep_Pawn();
    if (!_DelegatesBinded) BindDelegates();
    SetupInventoryWidget();
}

void APlayerControllerPlay::BindDelegates() {
    UNWGameInstance* GameInstance = Cast<UNWGameInstance>(GetGameInstance());
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (!GameInstance || !PlayerCharacter) return;

    if (PlayerCharacter->IsA(GameInstance->_PlayerInfoSaved.CharacterClass)) {
        PlayerCharacter->_OnRadioPressedDelegate.BindUObject(this, &APlayerControllerPlay::OnRadioPressed);
        PlayerCharacter->_OnRadioReleasedDelegate.BindUObject(this, &APlayerControllerPlay::OnRadioReleased);
        _DelegatesBinded = true;
    }
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
    if (PlayerCharacter) {
        if (_IsInventoryHidden && _IsMenuHidden) {
            PlayerCharacter->UseLeftPressed();
        }
        else {
            PlayerCharacter->FindComponentByClass<UWidgetInteractionComponent>()->
                PressPointerKey(EKeys::LeftMouseButton);
        }
    }
}

void APlayerControllerPlay::UseLeftReleased() {
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (PlayerCharacter) {
        if (_IsInventoryHidden && _IsMenuHidden) {
            PlayerCharacter->UseLeftReleased();
        }
        else {
            PlayerCharacter->FindComponentByClass<UWidgetInteractionComponent>()->
                ReleasePointerKey(EKeys::LeftMouseButton);
        }
    }
}

/******* USE ITEM RIGHT *********/
void APlayerControllerPlay::UseRightPressed() {
    if (_IsInventoryHidden) {
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
        if (PlayerCharacter) PlayerCharacter->UseRightPressed();
    }
}

void APlayerControllerPlay::UseRightReleased() {
    if (_IsInventoryHidden) {
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
        if (PlayerCharacter) PlayerCharacter->UseRightReleased();
    }
}

/*************** TRIGGER MENU *************/
void APlayerControllerPlay::ToogleMenu() {
    APawn* pawn = GetPawn();
    if (pawn) {
        if (_IsMenuHidden) {
            UCameraComponent* cameraComp = Cast<UCameraComponent>(pawn->FindComponentByClass<UCameraComponent>());
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


/**************** TRIGGER INVENTORY *************/
/*** SHOW INVENTORY ***/
void APlayerControllerPlay::ToggleInventory() {
    if (GetPawn() && _InventoryWidget) {
        if (_IsInventoryHidden) {
            _InventoryWidget->SetVisibility(ESlateVisibility::Visible);
            this->bShowMouseCursor = true;
            this->bEnableClickEvents = true;
            this->bEnableMouseOverEvents = true;

            FInputModeGameAndUI Mode = FInputModeGameAndUI();
            Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
            Mode.SetWidgetToFocus(_InventoryWidget->TakeWidget());
            SetInputMode(Mode);
        }
        else {
            _InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
            this->bShowMouseCursor = false;
            this->bEnableClickEvents = false;
            this->bEnableMouseOverEvents = false;
            SetInputMode(FInputModeGameOnly());
        }
        _IsInventoryHidden = !_IsInventoryHidden;
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