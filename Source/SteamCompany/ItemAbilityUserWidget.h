// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemAbilityUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEAMCOMPANY_API UItemAbilityUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FText NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FText ButtonText;

	UFUNCTION(BlueprintImplementableEvent)
		void StartCooldown(float Seconds);
};
