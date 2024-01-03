// Fill out your copyright notice in the Description page of Project Settings.

#include "EOS_GameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h"

void UEOS_GameInstance::LoginWithEOS(FString ID, FString Token, FString LoginType)
{
    IOnlineSubsystem* SubsystemReference = Online::GetSubsystem(this->GetWorld());
    if (SubsystemReference)
    {
        IOnlineIdentityPtr identityPointerRef = SubsystemReference->GetIdentityInterface();
        if (identityPointerRef)
        {
            FOnlineAccountCredentials AccountDetails;
            AccountDetails.Id = ID;
            AccountDetails.Token = Token;
            AccountDetails.Type = LoginType;

            identityPointerRef->OnLoginCompleteDelegates->AddUObject(this, &UEOS_GameInstance::LoginWithEOS_Return);
            identityPointerRef->Login(0, AccountDetails);
        }
    }
}

FString UEOS_GameInstance::GetPlayerUsername()
{
    IOnlineSubsystem* SubsystemReference = Online::GetSubsystem(this->GetWorld());

    if (SubsystemReference)
    {
        IOnlineIdentityPtr identityPointerRef = SubsystemReference->GetIdentityInterface();
        if (identityPointerRef)
        {
            if (identityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
            {
                return identityPointerRef->GetPlayerNickname(0);
            }
        }
    }
    return FString();
}

bool UEOS_GameInstance::IsPlayerLoggedIn()
{
    IOnlineSubsystem* SubsystemReference = Online::GetSubsystem(this->GetWorld());
    if (SubsystemReference)
    {
        IOnlineIdentityPtr identityPointerRef = SubsystemReference->GetIdentityInterface();
        if (identityPointerRef)
        {
            if (identityPointerRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
            {
                return true;
            }
        }
    }
    return false;
}

void UEOS_GameInstance::LoginWithEOS_Return(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error)
{
    if (bWasSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Login Success"))
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Login Fail Reason - %s"), *Error)
    }
}

void UEOS_GameInstance::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Warning, TEXT("ServerTravel"))
            GetWorld()->ServerTravel(OpenLevelText);
    }
}

void UEOS_GameInstance::OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful)
{
}

void UEOS_GameInstance::OnFindSessionCompleted(bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
        if (SubsystemRef)
        {
            IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
            if (SessionPtrRef)
            {
                if (SessionSearch->SearchResults.Num() > 0)
                {
                    SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnJoinSesssionCompleted);
                    SessionPtrRef->JoinSession(0, FName("MainSession"), SessionSearch->SearchResults[0]);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("0 SearchResults"));
                    // CreateEOSSession(false, false, 4);
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Unable to find session"));
        // CreateEOSSession(false, false, 4);
    }
}

void UEOS_GameInstance::OnJoinSesssionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        if (APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        {
            FString JoinAddress;
            IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
            if (SubsystemRef)
            {
                IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
                if (SessionPtrRef)
                {
                    SessionPtrRef->GetResolvedConnectString(FName("MainSession"), JoinAddress);
                    UE_LOG(LogTemp, Warning, TEXT("JoinAddress is %s"), *JoinAddress);
                    if (!JoinAddress.IsEmpty())
                    {
                        PlayerControllerRef->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
                    }
                }
            }
        }
    }
}

void UEOS_GameInstance::CreateEOSSession(bool bIsDeticatedServer, bool bIsLanServer, int32 NumberOfPublicConnections)
{
    IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
    if (SubsystemRef)
    {
        IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
        if (SessionPtrRef)
        {
            FOnlineSessionSettings SessionCreationInfo;
            SessionCreationInfo.bIsDedicated = bIsDeticatedServer;
            SessionCreationInfo.bAllowInvites = true;
            SessionCreationInfo.bIsLANMatch = bIsLanServer;
            SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
            SessionCreationInfo.bUseLobbiesIfAvailable = false;
            SessionCreationInfo.bUseLobbiesVoiceChatIfAvailable = false;
            SessionCreationInfo.bUsesPresence = false;
            SessionCreationInfo.bShouldAdvertise = true;
            SessionCreationInfo.Set(SEARCH_KEYWORDS, FString("RandomHi"), EOnlineDataAdvertisementType::ViaOnlineService);
            SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnCreateSessionCompleted);
            SessionPtrRef->CreateSession(0, FName("MainSession"), SessionCreationInfo);
        }
    }
}

void UEOS_GameInstance::FindSessionAndJoin()
{
    IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
    if (SubsystemRef)
    {
        IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
        if (SessionPtrRef)
        {
            SessionSearch = MakeShareable(new FOnlineSessionSearch());
            //SessionSearch->bIsLanQuery = false;
            //SessionSearch->QuerySettings.SearchParams.Empty();
            //SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, false, EOnlineComparisonOp::Equals);
            SessionSearch->MaxSearchResults = 20;
            SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnFindSessionCompleted);
            SessionPtrRef->FindSessions(0, SessionSearch.ToSharedRef());
        }
    }
}

void UEOS_GameInstance::JoinSession()
{
}

void UEOS_GameInstance::DestroySession()
{
    IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(this->GetWorld());
    if (SubsystemRef)
    {
        IOnlineSessionPtr SessionPtrRef = SubsystemRef->GetSessionInterface();
        if (SessionPtrRef)
        {
            SessionPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnDestroySessionCompleted);
            SessionPtrRef->DestroySession(FName("MainSession"));
        }
    }
}
