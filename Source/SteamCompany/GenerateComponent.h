// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GenerateComponent.generated.h"

class ULevelStreamingDynamic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelGenerateComponentComplete);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMCOMPANY_API UGenerateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGenerateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION()
    void OnBeginGenerateLevel();

	UFUNCTION()
	void LoadLevelAtPosition(UWorld* World, const TSoftObjectPtr<UWorld>& MapAsset, const FVector& Position, const FRotator& Rotation);

	UPROPERTY()
	int32 LevelIndex = 0;

	// In your UGenerateComponent class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 SeedOffset = 5;

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
	TSubclassOf<ULevelStreamingDynamic> LevelStreamingClass;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelGenerateComponentComplete OnLevelGenerateComplete;

};
