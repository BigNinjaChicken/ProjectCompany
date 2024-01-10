// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffectComponent.h"
#include "CombatComponent.h"
#include "PhysDmgIncreaseComponent.generated.h"

class UCombatComponent;

/**
 * 
 */
UCLASS()
class STEAMCOMPANY_API UPhysDmgIncreaseComponent : public UItemEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPhysDmgIncreaseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void UpdateStats(int32 ChangeInItemAmount) override;

	UCombatComponent* CombatComponent;
};
