// Fill out your copyright notice in the Description page of Project Settings.

#include "LTMV.h"
#include "NWGameInstance.h"

#include "PlayerCharacter.h"


void UNWGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UNWGameInstance, _MaxPlayers);
    DOREPLIFETIME(UNWGameInstance, _ServerName);
}

UNWGameInstance::UNWGameInstance(const FObjectInitializer& OI) : Super(OI) {
    /** Bind function for CREATING a Session */
    OnCreateSessionCompleteDelegate =
        FOnCreateSessionCompleteDelegate::CreateUObject(this, &UNWGameInstance::OnCreateSessionComplete);
    OnStartSessionCompleteDelegate =
        FOnStartSessionCompleteDelegate::CreateUObject(this, &UNWGameInstance::OnStartOnlineGameComplete);

    /** Bind function for FINDING a Session */
    OnFindSessionsCompleteDelegate =
        FOnFindSessionsCompleteDelegate::CreateUObject(this, &UNWGameInstance::OnFindSessionsComplete);

    /** Bind function for JOINING a Session */
    OnJoinSessionCompleteDelegate =
        FOnJoinSessionCompleteDelegate::CreateUObject(this, &UNWGameInstance::OnJoinSessionComplete);
    
    /** Bind function for DESTROYING a Session */
    OnDestroySessionCompleteDelegate =
        FOnDestroySessionCompleteDelegate::CreateUObject(this, &UNWGameInstance::OnDestroySessionComplete);

    _MapLobbyName = USettings::Get()->LevelLobby.GetAssetName();
    
    _MaxPlayers = 2;
    _ServerName = "";
    _SessionOwner = "";

    /* MENU INTERFACE */
    static ConstructorHelpers::FClassFinder<AActor> MenuClassFinder(TEXT(
        "/Game/BluePrints/HUD/Menu_BP"));
    _MenuClass = MenuClassFinder.Class;
    _IsMenuHidden = true;
}

IOnlineSessionPtr UNWGameInstance::GetSessions() {
    IOnlineSessionPtr Sessions;
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub) {
        Sessions = OnlineSub->GetSessionInterface();
    }
    else {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
    }
    return Sessions;
}

void UNWGameInstance::InitGame() {
    APlayerController* const PlayerController = GetFirstLocalPlayerController();
    if (PlayerController) {
        APawn* Pawn = PlayerController->GetPawn();
        if (Pawn) {
            UCameraComponent* CameraComp = Cast<UCameraComponent>(Pawn->FindComponentByClass<UCameraComponent>());
            if (CameraComp) {
                ToogleMenu(CameraComp->GetComponentLocation(),
                           CameraComp->GetComponentRotation());

                APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
                if (PlayerCharacter) PlayerCharacter->ToggleMenuInteraction(!_IsMenuHidden);
            }
        }
    }
}

/**************************************** BLUEPRINTS *********************************************/
void UNWGameInstance::LaunchLobby(FName ServerName) {
    DestroySession();

    _ServerName = ServerName;
    ULocalPlayer* const Player = GetFirstGamePlayer();
    HostSession(Player->GetPreferredUniqueNetId(), GameSessionName, true, true, _MaxPlayers);
}

void UNWGameInstance::FindOnlineGames() {
    ULocalPlayer* const Player = GetFirstGamePlayer();
    FindSessions(Player->GetPreferredUniqueNetId(), true, true);
}

void UNWGameInstance::JoinOnlineGame() {
    ULocalPlayer* const Player = GetFirstGamePlayer();
    FOnlineSessionSearchResult SearchResult;
    if (_SessionSearch->SearchResults.Num() > 0) {
        for (int32 i = 0; i < _SessionSearch->SearchResults.Num(); i++) {
            SearchResult = _SessionSearch->SearchResults[i];
            if (SearchResult.Session.OwningUserId != Player->GetPreferredUniqueNetId()) {
                JoinAtSession(Player->GetPreferredUniqueNetId(), GameSessionName, SearchResult);
                break;
            }
        }
    }
}

void UNWGameInstance::DestroySession() {
    IOnlineSessionPtr Sessions = GetSessions();
    if (Sessions.IsValid()) {
        Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
        Sessions->DestroySession(GameSessionName);
    }
}

/**************************************** SESSION ************************************************/
bool UNWGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName,
                                  bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers) {
    IOnlineSessionPtr Sessions = GetSessions();
    if (Sessions.IsValid() && UserId.IsValid()) {
        _SessionSettings = MakeShareable(new FOnlineSessionSettings());

        _SessionSettings->bIsLANMatch = bIsLAN;
        _SessionSettings->bUsesPresence = bIsPresence;
        _SessionSettings->NumPublicConnections = MaxNumPlayers;
        _SessionSettings->NumPrivateConnections = 0;
        _SessionSettings->bAllowInvites = true;
        _SessionSettings->bAllowJoinInProgress = true;
        _SessionSettings->bShouldAdvertise = true;
        _SessionSettings->bAllowJoinViaPresence = true;
        _SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
        _SessionSettings->Set(SETTING_MAPNAME, _MapLobbyName,
                              EOnlineDataAdvertisementType::ViaOnlineService);
        OnCreateSessionCompleteDelegateHandle = 
            Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

        return Sessions->CreateSession(*UserId, SessionName, *_SessionSettings);
    }
    return false;
}

void UNWGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
    IOnlineSessionPtr Sessions = GetSessions();
    if (Sessions.IsValid()) {
        Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
        OnStartSessionCompleteDelegateHandle =
            Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
        Sessions->StartSession(GameSessionName);
    }
}

void UNWGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful) {
    IOnlineSessionPtr Sessions = GetSessions();
    if (Sessions.IsValid()) {
        Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
    }
    if (bWasSuccessful) {
        UGameplayStatics::OpenLevel(GetWorld(), FName(*_MapLobbyName), true, "listen");
    }
}

void UNWGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence) {
    IOnlineSessionPtr Sessions = GetSessions();
    if (Sessions.IsValid() && UserId.IsValid()) {
        _SessionSearch = MakeShareable(new FOnlineSessionSearch());
        _SessionSearch->bIsLanQuery = bIsLAN;
        _SessionSearch->MaxSearchResults = 20;
        _SessionSearch->PingBucketSize = 50;
        _SessionSearch->TimeoutInSeconds = 15;

        //if (bIsPresence) { 
        //    _SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
        //}

        TSharedRef<FOnlineSessionSearch> SearchSettingsRef = _SessionSearch.ToSharedRef();
        OnFindSessionsCompleteDelegateHandle = 
            Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
        _SessionOwner = "";
        Sessions->FindSessions(*UserId, SearchSettingsRef);
    }
}

void UNWGameInstance::OnFindSessionsComplete(bool bWasSuccessful) {
    ULibraryUtils::Log(FString::Printf(TEXT("Number of Sessions found: %d"),
                                       _SessionSearch->SearchResults.Num()), 3, 15);
    IOnlineSessionPtr Sessions = GetSessions();
    FString Result = "";
    if (Sessions.IsValid()) {
        Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
        for (int32 i = 0; i < _SessionSearch->SearchResults.Num(); i++) {
            if (_SessionSearch->SearchResults[i].Session.NumOpenPublicConnections > 0) {
                Result = _SessionSearch->SearchResults[i].Session.OwningUserName;
            }
        }
    }
    else Result = "NO VALID SESSIONS";

    _SessionOwner = Result.Len() > 0 ? Result : "NO SESSIONS FOUND";
}

bool UNWGameInstance::JoinAtSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName,
                                    const FOnlineSessionSearchResult& SearchResult) {
    bool bSuccessful = false;
    IOnlineSessionPtr Sessions = GetSessions();
    if (Sessions.IsValid() && UserId.IsValid()) {
        OnJoinSessionCompleteDelegateHandle = 
            Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
        bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
    }
    return bSuccessful;
}

void UNWGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
    IOnlineSessionPtr Sessions = GetSessions();
    if (Sessions.IsValid()) {
        Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
        APlayerController * const PlayerController = GetFirstLocalPlayerController();
        FString TravelURL;
        if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL)) {
            PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
        }
    }
}

void UNWGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful) {
    IOnlineSessionPtr Sessions = GetSessions();
    if (Sessions.IsValid()) {
        Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
        if (bWasSuccessful) {
            //UGameplayStatics::OpenLevel(GetWorld(), _MapMenuName, true);
            APlayerController * const PlayerController = GetFirstLocalPlayerController();
            if (PlayerController) PlayerController->ClientReturnToMainMenu("");
        }
    }
}


/************************************ MENU INTERFACE *********************************************/
void UNWGameInstance::ToogleMenu(FVector Location, FRotator Rotation) {
    if (_IsMenuHidden) {
        if (_MenuActor) {
            ULibraryUtils::SetActorEnable(_MenuActor);
            _MenuActor->SetActorLocationAndRotation(Location,
                                                    Rotation,
                                                    false,
                                                    nullptr,
                                                    ETeleportType::TeleportPhysics);
        }
        else {
            _MenuActor = GetWorld()->SpawnActor(_MenuClass, &Location, &Rotation);
        }
    }
    else {
        ULibraryUtils::SetActorEnable(_MenuActor, false);
    }
    _IsMenuHidden = !_IsMenuHidden;
}