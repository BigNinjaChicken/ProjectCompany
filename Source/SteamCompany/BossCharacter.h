// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItemPickupActor.h"
#include "BossCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDead);

class AItemPickupActor;

UCLASS()
class STEAMCOMPANY_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void HandleActorDead();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBossDead OnBossDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	TArray<TSubclassOf<AItemPickupActor>> DropedItemPossibilites;

	bool bHasDroppedItem = false;
};
