// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActiveItemsUserWidget.generated.h"

class UGridPanel;

/**
 * 
 */
UCLASS()
class STEAMCOMPANY_API UActiveItemsUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UGridPanel* ItemGridPanel;
};
