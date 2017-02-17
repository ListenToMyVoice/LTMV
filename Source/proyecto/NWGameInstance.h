// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "NWGameInstance.generated.h"


UCLASS()
class PROYECTO_API UNWGameInstance : public UGameInstance {
    GENERATED_BODY()

public:
    TSharedPtr<class FOnlineSessionSettings> SessionSettings;
    TSharedPtr<class FOnlineSessionSearch> SessionSearch;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    FString SessionsList;
    UPROPERTY(BlueprintReadOnly, Category = "Session")
    FName _MapName;

    UNWGameInstance(const FObjectInitializer& OI);

    /*********************************** BLUEPRINTS **********************************************/
    UFUNCTION(BlueprintCallable, Category = "Session")
    void StartOnlineGame(FName SessionName);
    UFUNCTION(BlueprintCallable, Category = "Session")
    void FindOnlineGames();
    UFUNCTION(BlueprintCallable, Category = "Session")
    void JoinOnlineGame(FName SessionName);
    UFUNCTION(BlueprintCallable, Category = "Session")
    void DestroySessionAndLeaveGame(FName SessionName);

    /************************************ SESSION ************************************************/
    /**
    *	Function to host a game!
    *
    *	@Param	UserID - User that started the request
    *	@Param	SessionName - Name of the Session
    *	@Param	bIsLAN - Is this is LAN Game?
    *	@Param	bIsPresence - "Is the Session to create a presence Session"
    *	@Param	MaxNumPlayers - Number of Maximum allowed players on this "Session" (Server)
    */
    bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN,
                     bool bIsPresence, int32 MaxNumPlayers);

    /**
    *	Find an online session
    *
    *	@param UserId user that initiated the request
    *	@param SessionName name of session this search will generate
    *	@param bIsLAN are we searching LAN matches
    *	@param bIsPresence are we searching presence sessions
    */
    void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

    /**
    *	Joins a session via a search result
    *
    *	@param SessionName name of session
    *	@param SearchResult Session to join
    *
    *	@return bool true if successful, false otherwise
    */
    bool JoinAtSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName,
                       const FOnlineSessionSearchResult& SearchResult);

    /* Delegate called when session created */
    FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
    FDelegateHandle OnCreateSessionCompleteDelegateHandle;
    virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

    /* Delegate called when session started */
    FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
    FDelegateHandle OnStartSessionCompleteDelegateHandle;
    void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);


    /* Delegate for searching for sessions */
    FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
    FDelegateHandle OnFindSessionsCompleteDelegateHandle;
    void OnFindSessionsComplete(bool bWasSuccessful);

    /* Delegate for joining a session */
    FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
    FDelegateHandle OnJoinSessionCompleteDelegateHandle;
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    /* Delegate for destroying a session */
    FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
    FDelegateHandle OnDestroySessionCompleteDelegateHandle;
    virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

    /************************************** VOICE ************************************************/
    //void RegisterVoice();
};
