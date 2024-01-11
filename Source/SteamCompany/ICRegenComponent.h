// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemEffectComponent.h"
#include "CombatComponent.h"

#include "ICRegenComponent.generated.h"

class UCombatComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UICRegenComponent : public UItemEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UICRegenComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void UpdateStats(int32 ChangeInItemAmount) override;

	UPROPERTY()
	UCombatComponent* CombatComponent;

	// Amount healed after HealSpeedTime
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealAmount = 1.5f;

	// In Seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealSpeedTime = 1.0f;

	// On EnterCombat
	UFUNCTION()
	void HandleEnterCombat();

	// On ExitCombat
	UFUNCTION()
	void HandleExitCombat();

	UFUNCTION()
	void HealPlayer();

	FTimerHandle CombatTimerHandle;
};
