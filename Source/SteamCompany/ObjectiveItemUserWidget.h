// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveItemUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEAMCOMPANY_API UObjectiveItemUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
	FText ItemText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Objective)
	ECheckBoxState ItemCheckBoxState;
};
