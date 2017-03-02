// Fill out your copyright notice in the Description page of Project Settings.

#include "proyecto.h"
#include "NWGameInstance.h"


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

    _MapLobbyName = FName(TEXT("Lobby"));
    _MaxPlayers = 2;
    _ServerName = "";

    static ConstructorHelpers::FClassFinder<AActor> ClassFinder(TEXT("/Game/Maps/Alex/MenuActor"));
    _MenuClass = ClassFinder.Class;
    _Menu = nullptr;
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
    CreateMenu();
    GetPrimaryPlayerController()->bShowMouseCursor = true;
}

/**************************************** BLUEPRINTS *********************************************/
void UNWGameInstance::LaunchLobby(FName ServerName) {
    _ServerName = ServerName;
    ULocalPlayer* const Player = GetFirstGamePlayer();
    HostSession(Player->GetPreferredUniqueNetId(), GameSessionName, true, false, _MaxPlayers);
}

void UNWGameInstance::CreateOnlineGame() {
    // Creating a local player where we can get the UserID from
    ULocalPlayer* const Player = GetFirstGamePlayer();

    // Call our custom HostSession function. GameSessionName is a GameInstance variable
    HostSession(Player->GetPreferredUniqueNetId(), GameSessionName, true, false, 2);
}

void UNWGameInstance::StartOnlineGame() {
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub) {
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
            // Set the StartSession delegate handle
            OnStartSessionCompleteDelegateHandle = 
                Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

            // Our StartSessionComplete delegate should get called after this
            Sessions->StartSession(GameSessionName);
        }
    }
}

void UNWGameInstance::FindOnlineGames() {
    ULocalPlayer* const Player = GetFirstGamePlayer();

    FindSessions(Player->GetPreferredUniqueNetId(), true, true);
}

void UNWGameInstance::JoinOnlineGame() {
    ULocalPlayer* const Player = GetFirstGamePlayer();

    // Just a SearchResult where we can save the one we want to use, for the case we find more than one!
    FOnlineSessionSearchResult SearchResult;

    // If the Array is not empty, we can go through it
    if (_SessionSearch->SearchResults.Num() > 0) {
        for (int32 i = 0; i < _SessionSearch->SearchResults.Num(); i++) {
            // To avoid something crazy, we filter sessions from ourself
            if (_SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId()) {
                SearchResult = _SessionSearch->SearchResults[i];

                // Once we found sounce a Session that is not ours, just join it. Instead of using a for loop, you could
                // use a widget where you click on and have a reference for the GameSession it represents which you can use
                // here
                JoinAtSession(Player->GetPreferredUniqueNetId(), GameSessionName, SearchResult);
                break;
            }
        }
    }
}

void UNWGameInstance::DestroySessionAndLeaveGame() {
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub) {
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
            Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

            Sessions->DestroySession(GameSessionName);
        }
    }
}


/**************************************** SESSION ************************************************/
bool UNWGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName,
                                  bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers) {
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("df")));

    IOnlineSessionPtr Sessions = GetSessions();
    if (Sessions.IsValid() && UserId.IsValid()) {
        _SessionSettings = MakeShareable(new FOnlineSessionSettings());

        _SessionSettings->bIsLANMatch = bIsLAN;
        //_SessionSettings->bUsesPresence = bIsPresence;
        _SessionSettings->NumPublicConnections = MaxNumPlayers;
        _SessionSettings->NumPrivateConnections = 0;
        //_SessionSettings->bAllowInvites = true;
        //_SessionSettings->bAllowJoinInProgress = true;
        //_SessionSettings->bShouldAdvertise = true;
        //_SessionSettings->bAllowJoinViaPresence = true;
        //_SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
        //_SessionSettings->Set(SETTING_MAPNAME, FString("test_map"), EOnlineDataAdvertisementType::ViaOnlineService);
        OnCreateSessionCompleteDelegateHandle = 
            Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

        return Sessions->CreateSession(*UserId, SessionName, *_SessionSettings);
    }
    return false;
}

void UNWGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT(
        "OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));
    IOnlineSessionPtr Sessions = GetSessions();
    if (Sessions.IsValid()) {
        Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
        UGameplayStatics::OpenLevel(GetWorld(), _MapLobbyName, true, "listen");
    }
}












void UNWGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful) {
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

    // Get the Online Subsystem so we can get the Session Interface
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub) {
        // Get the Session Interface to clear the Delegate
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
        if (Sessions.IsValid()) {
            // Clear the delegate, since we are done with this call
            Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
        }
    }

    // If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
    if (bWasSuccessful) {
        UGameplayStatics::OpenLevel(GetWorld(), _MapLobbyName, true, "listen");
    }
}

void UNWGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence) {
    // Get the OnlineSubsystem we want to work with
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
        // Get the SessionInterface from our OnlineSubsystem
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid() && UserId.IsValid()) {
            /*
            Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
            */
            _SessionSearch = MakeShareable(new FOnlineSessionSearch());

            _SessionSearch->bIsLanQuery = bIsLAN;
            _SessionSearch->MaxSearchResults = 20;
            _SessionSearch->PingBucketSize = 50;

            // We only want to set this Query Setting if "bIsPresence" is true
            if (bIsPresence) {
                _SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
            }

            TSharedRef<FOnlineSessionSearch> SearchSettingsRef = _SessionSearch.ToSharedRef();

            // Set the Delegate to the Delegate Handle of the FindSession function
            OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

            // Finally call the SessionInterface function. The Delegate gets called once this is finished
            Sessions->FindSessions(*UserId, SearchSettingsRef);
        }
    }
    else {
        // If something goes wrong, just call the Delegate Function directly with "false".
        OnFindSessionsComplete(false);
    }
}

void UNWGameInstance::OnFindSessionsComplete(bool bWasSuccessful) {
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

    // Get OnlineSubsystem we want to work with
    IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub) {
        // Get SessionInterface of the OnlineSubsystem
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
        if (Sessions.IsValid()) {
            // Clear the Delegate handle, since we finished this call
            Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

            // Just debugging the Number of Search results. Can be displayed in UMG or something later on
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), _SessionSearch->SearchResults.Num()));

            // If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
            if (_SessionSearch->SearchResults.Num() > 0) {
                // "_SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
                // This can be customized later on with your own classes to add more information that can be set and displayed
                for (int32 SearchIdx = 0; SearchIdx < _SessionSearch->SearchResults.Num(); SearchIdx++) {
                    // OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
                    // This is something you can't do in Blueprint for example!
                    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                               FString::Printf(TEXT("Session Number: %d | Sessionname: %s "),
                               SearchIdx + 1,
                               *(_SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
                }
            }
        }
    }
}

bool UNWGameInstance::JoinAtSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName,
                                    const FOnlineSessionSearchResult& SearchResult) {
    // Return bool
    bool bSuccessful = false;

    // Get OnlineSubsystem we want to work with
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

    if (OnlineSub) {
        // Get SessionInterface from the OnlineSubsystem
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid() && UserId.IsValid()) {
            // Set the Handle again
            OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

            // Call the "JoinSession" Function with the passed "SearchResult". The "_SessionSearch->SearchResults" can be used to get such a
            // "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
            bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
        }
    }

    return bSuccessful;
}

void UNWGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));

    // Get the OnlineSubsystem we want to work with
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub) {
        // Get SessionInterface from the OnlineSubsystem
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
            // Clear the Delegate again
            Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

            // Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
            // This is something the Blueprint Node "Join Session" does automatically!
            APlayerController * const PlayerController = GetFirstLocalPlayerController();

            // We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
            // String for us by giving him the SessionName and an empty String. We want to do this, because
            // Every OnlineSubsystem uses different TravelURLs
            FString TravelURL;

            if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL)) {
                // Finally call the ClienTravel. If you want, you could print the TravelURL to see
                // how it really looks like
                PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
            }
        }
    }
}

void UNWGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful) {
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

    // Get the OnlineSubsystem we want to work with
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub) {
        // Get the SessionInterface from the OnlineSubsystem
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

        if (Sessions.IsValid()) {
            // Clear the Delegate
            Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

            // If it was successful, we just load another level (could be a MainMenu!)
            if (bWasSuccessful) {
                //_SessionName = "";
                UGameplayStatics::OpenLevel(GetWorld(), "Menu", true);
            }
        }
    }
}

/*************************************** MAIN MENU ***********************************************/
void UNWGameInstance::CreateMenu() {
    FVector position(0.0f, 0.0f, 200.0f);
    FRotator rotation(0.0f, 0.0f, 0.0f);
    _Menu = GetWorld()->SpawnActor(_MenuClass, &position, &rotation);
}