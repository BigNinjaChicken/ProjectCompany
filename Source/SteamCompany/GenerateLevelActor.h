// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ChildActorComponent.h"
#include "GenerateLevelActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelGenerateComplete);

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

    void OnBeginGenerateLevel();

	void LoadLevelAtPosition(UWorld* World, const TSoftObjectPtr<UWorld>& MapAsset, const FVector& Position, const FRotator& Rotation);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Grid generation function
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Grid")
	void GenerateGrid();

	UPROPERTY(Replicated)
	int32 LevelIndex;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 Indexl;

	// Grid dimensions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 GridSizeX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 GridSizeY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float GridSpacing;

	// SubActor class to use for components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TSubclassOf<AActor> SubActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> StraightLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> CornerLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> FourWayIntersectionLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> ThreeWayIntersectionLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> EndCapLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> ChurchStraightLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> ChurchCornerLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> ChurchFourWayIntersectionLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> ChurchThreeWayIntersectionLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<TSoftObjectPtr<UWorld>> ChurchEndCapLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float ScaleFactor = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 LevelNameIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	ULevelStreamingDynamic* DynamicLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TSubclassOf<ULevelStreamingDynamic> LevelStreamingClass;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelGenerateComplete OnLevelGenerateComplete;
};
