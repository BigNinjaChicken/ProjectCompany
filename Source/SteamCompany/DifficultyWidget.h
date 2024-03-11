// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../../../../Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "DifficultyWidget.generated.h"

USTRUCT(BlueprintType)
struct FDifficulty
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Difficulty")
	FString DifficultyName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Difficulty")
	FSlateBrush DifficultyColor;
};

/**
 * 
 */
UCLASS()
class STEAMCOMPANY_API UDifficultyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WBP")
	int World = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WBP")
	FText GameTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WBP")
	TArray<FDifficulty> Difficulties;

	UFUNCTION(BlueprintImplementableEvent)
	void SetDifficulty(int NewDifficulty);
};
