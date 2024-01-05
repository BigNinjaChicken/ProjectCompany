// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ChildActorComponent.h"
#include "GenerateLevelActor.generated.h"

class ULevelStreamingDynamic;

UCLASS()
class STEAMCOMPANY_API AGenerateLevelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerateLevelActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Replication setup
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
    void ServerOnBeginGenerateLevel();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastOnBeginGenerateLevel();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLoadLevelAtPosition(UWorld* World, const TSoftObjectPtr<UWorld>& MapAsset, const FVector& Position, const FRotator& Rotation);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Grid generation function
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Grid")
	void GenerateGrid();

	// Grid dimensions
	UPROPERTY(EditAnywhere, Category = "Grid")
	int32 GridSizeX;

	UPROPERTY(EditAnywhere, Category = "Grid")
	int32 GridSizeY;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float GridSpacing;

	// SubActor class to use for components
	UPROPERTY(EditAnywhere, Category = "Grid")
	TSubclassOf<AActor> SubActorClass;

	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> StraightLevels;

	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> CornerLevels;

	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> FourWayIntersectionLevels;

	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> ThreeWayIntersectionLevels;

	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> EndCapLevels;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float ScaleFactor = 100.0f;

	int32 LevelNameIndex = 0;

	ULevelStreamingDynamic* DynamicLevel;
};
