// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossBarComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossSpawned, UCombatComponent*, CombatComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossTakeDamage, UCombatComponent*, CombatComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDeadUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UBossBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBossBarComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FOnBossSpawned OnBossSpawned;
	UPROPERTY(BlueprintAssignable)
		FOnBossTakeDamage OnBossTakeDamage;
	UPROPERTY(BlueprintAssignable)
		FOnBossDeadUpdated OnBossDead;

	UFUNCTION()
		void BossSpawned();
	UFUNCTION()
		void BossTakeDamage();
	UFUNCTION()
		void BossDead();

	UCombatComponent* BossCombatComponent;
	bool bBossSpawned = false;
};
