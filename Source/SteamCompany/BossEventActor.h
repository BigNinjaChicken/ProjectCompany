// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "BossEventActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginBossEvent);

UCLASS()
class STEAMCOMPANY_API ABossEventActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossEventActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UFUNCTION()
	void BeginBossEvent();

	UFUNCTION()
	void SpawnBoss();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* BossArenaSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BossArenaStaticMeshComponent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBeginBossEvent OnBeginBossEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	TSubclassOf<ACharacter> BossCharacter;

	// Delay in seconds before spawning the boss
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float SpawnDelay = 2.0f;
};
