// Fill out your copyright notice in the Description page of Project Settings.

#include "EOS_GameMode.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

void AEOS_GameMode::BeginPlay()
{
    Super::BeginPlay();
}

void AEOS_GameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (NewPlayer)
    {
        FUniqueNetIdRepl UniqueNetIdRepl;
        if (NewPlayer->IsLocalPlayerController()) // Host of server in GameMode
        {
            ULocalPlayer* LocalPlayerRef = NewPlayer->GetLocalPlayer();
            if (LocalPlayerRef)
            {
                UniqueNetIdRepl = LocalPlayerRef->GetPreferredUniqueNetId();
            }
            else
            {
                UNetConnection* RemoteNetConnectionRef = Cast<UNetConnection>(NewPlayer->Player);
                check(IsValid(RemoteNetConnectionRef));
                UniqueNetIdRepl = RemoteNetConnectionRef->PlayerId;
            }
        }
        else
        {
            UNetConnection* RemoteNetConnectionRef = Cast<UNetConnection>(NewPlayer->Player);
            check(IsValid(RemoteNetConnectionRef));
            UniqueNetIdRepl = RemoteNetConnectionRef->PlayerId;
        }

        TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIdRepl.GetUniqueNetId();
        check(UniqueNetId != nullptr);
        IOnlineSubsystem* SubsystemRef = Online::GetSubsystem(NewPlayer->GetWorld());
        IOnlineSessionPtr SessionRef = SubsystemRef->GetSessionInterface();
        bool bRregistartionSuccess = SessionRef->RegisterPlayer(FName("MainSession"), *UniqueNetId, false);
        if (bRregistartionSuccess)
        {
            UE_LOG(LogTemp, Warning, TEXT("Registration Success"));
        }
    }
}
