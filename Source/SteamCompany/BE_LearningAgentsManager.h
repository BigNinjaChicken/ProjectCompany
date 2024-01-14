// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LearningAgentsManager.h"
#include "BE_LearningAgentsManager.generated.h"

/**
 * 
 */
UCLASS()
class STEAMCOMPANY_API ABE_LearningAgentsManager : public ALearningAgentsManager
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ABE_LearningAgentsManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
