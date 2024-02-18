// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Components/VerticalBoxSlot.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Components/VerticalBox.h"
#include "ObjectiveUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEAMCOMPANY_API UObjectiveUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UVerticalBox* ObjectiveVerticalBox;
};
