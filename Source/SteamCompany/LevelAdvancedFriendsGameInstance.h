// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "LevelAdvancedFriendsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STEAMCOMPANY_API ULevelAdvancedFriendsGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 CurrentLevel = 1;
};
