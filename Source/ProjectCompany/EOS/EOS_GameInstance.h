// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "EOS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCOMPANY_API UEOS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EOSFunction")
	void LoginWithEOS(FString ID, FString Token, FString LoginType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSFunction")
	FString GetPlayerUsername();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSFunction")
	bool IsPlayerLoggedIn();

	UFUNCTION(BlueprintCallable, Category = "EOSFunction")
	void CreateEOSSession(bool bIsDeticatedServer, bool bIsLanServer, int32 NumberOfPublicConnections);

	UFUNCTION(BlueprintCallable, Category = "EOSFunction")
	void FindSessionAndJoin();

	UFUNCTION(BlueprintCallable, Category = "EOSFunction")
	void JoinSession();

	UFUNCTION(BlueprintCallable, Category = "EOSFunction")
	void DestroySession();

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOSFunction")
	FString OpenLevelText;

	void LoginWithEOS_Return(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId &UserId, const FString &Error);
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnFindSessionCompleted(bool bWasSuccessful);
	void OnJoinSesssionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
