// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DifficultyWidget.h"
#include "DifficultySystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UDifficultySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDifficultySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void GameTimeUpdate(float NewTime);
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WBP")
	TSubclassOf<UDifficultyWidget> DifficultyWidgetClass;

	UDifficultyWidget* DifficultyWidget;

	int CurrentDifficulty = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WBP")
	int DifficultyTextInterval = 120;
};
