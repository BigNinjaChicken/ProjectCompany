// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EOS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCOMPANY_API AEOS_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
