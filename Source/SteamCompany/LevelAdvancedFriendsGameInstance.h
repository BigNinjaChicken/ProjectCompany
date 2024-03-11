// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "ItemEffectComponent.h"
#include "LevelAdvancedFriendsGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TSubclassOf<UItemEffectComponent> ItemEffectComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 ItemCount;
};

USTRUCT(BlueprintType)
struct FItemEffectComponentArray
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TArray<FItemData> PlayerItems;
};


UCLASS()
class STEAMCOMPANY_API ULevelAdvancedFriendsGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 CurrentLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float CurrentGameTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TMap<FString, FItemEffectComponentArray> PlayerItemsMap;
};
