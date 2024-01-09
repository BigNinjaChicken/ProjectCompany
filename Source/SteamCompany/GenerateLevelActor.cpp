// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateLevelActor.h"
#include "TileActor.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "LevelInstance/LevelInstanceLevelStreaming.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AGenerateLevelActor::AGenerateLevelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void AGenerateLevelActor::BeginPlay()
{
	Super::BeginPlay();

	MulticastOnBeginGenerateLevel();
}

void AGenerateLevelActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGenerateLevelActor::MulticastOnBeginGenerateLevel_Implementation()
{
	TArray<AActor*> ChildActors;
	GetAttachedActors(ChildActors);

	for (AActor* ChildActor : ChildActors) {
		ATileActor* TileActor = Cast<ATileActor>(ChildActor);
		if (!TileActor) continue;

		// Example parameters - you need to provide actual parameters as per your game logic
		UWorld* World = GetWorld();
		TSoftObjectPtr<UWorld> MapAsset; // Example, replace with actual asset

		FVector Position = TileActor->GetActorLocation();
		Position.X *= ScaleFactor;
		Position.Y *= ScaleFactor;
		FRotator Rotation = TileActor->GetActorRotation();

		int32 LevelIndex;
		switch (TileActor->TileShape)
		{
		case ETileShape::Straight:
			// Handle Straight logic
			if (TileActor->bIsChurch) {
				LevelIndex = FMath::RandRange(0, ChurchStraightLevels.Num() - 1);
				LoadLevelAtPosition(World, ChurchStraightLevels[LevelIndex], Position, Rotation);
			}
			else {
				LevelIndex = FMath::RandRange(0, StraightLevels.Num() - 1);
				LoadLevelAtPosition(World, StraightLevels[LevelIndex], Position, Rotation);
			}
			break;
		case ETileShape::Corner:
			// Handle Corner logic
			if (TileActor->bIsChurch) {
				LevelIndex = FMath::RandRange(0, ChurchCornerLevels.Num() - 1);
				LoadLevelAtPosition(World, ChurchCornerLevels[LevelIndex], Position, Rotation);
			}
			else {
				LevelIndex = FMath::RandRange(0, CornerLevels.Num() - 1);
				LoadLevelAtPosition(World, CornerLevels[LevelIndex], Position, Rotation);
			}
			break;
		case ETileShape::ThreeWayIntersection:
			if (TileActor->bIsChurch) {
				LevelIndex = FMath::RandRange(0, ChurchThreeWayIntersectionLevels.Num() - 1);
				LoadLevelAtPosition(World, ChurchThreeWayIntersectionLevels[LevelIndex], Position, Rotation);
			}
			else {
				LevelIndex = FMath::RandRange(0, ThreeWayIntersectionLevels.Num() - 1);
				LoadLevelAtPosition(World, ThreeWayIntersectionLevels[LevelIndex], Position, Rotation);
			}
			break;
		case ETileShape::FourWayIntersection:
			// Handle FourWayIntersection logic
			if (TileActor->bIsChurch) {
				LevelIndex = FMath::RandRange(0, ChurchFourWayIntersectionLevels.Num() - 1);
				LoadLevelAtPosition(World, ChurchFourWayIntersectionLevels[LevelIndex], Position, Rotation);
			}
			else {
				LevelIndex = FMath::RandRange(0, FourWayIntersectionLevels.Num() - 1);
				LoadLevelAtPosition(World, FourWayIntersectionLevels[LevelIndex], Position, Rotation);
			}
			break;
		case ETileShape::EndCap:;
			// Handle EndCap logic
			if (TileActor->bIsChurch) {
				LevelIndex = FMath::RandRange(0, ChurchEndCapLevels.Num() - 1);
				LoadLevelAtPosition(World, ChurchEndCapLevels[LevelIndex], Position, Rotation);
			}
			else {
				LevelIndex = FMath::RandRange(0, EndCapLevels.Num() - 1);
				LoadLevelAtPosition(World, EndCapLevels[LevelIndex], Position, Rotation);
			}
			break;
		default:
			// Handle default case
			break;
		}
	}
}

// Function to load a level into the current level at a specified position and rotation
void AGenerateLevelActor::LoadLevelAtPosition(UWorld* World, const TSoftObjectPtr<UWorld>& MapAsset, const FVector& Position, const FRotator& Rotation)
{
	if (MapAsset.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("MapAsset is null"));
	}
	
	FString MapPath = MapAsset.GetLongPackageName();
	bool bOutSuccess;
	FString LevelName = "LevelTile" + FString::FromInt(LevelNameIndex);
	LevelNameIndex++;
	DynamicLevel = ULevelStreamingDynamic::LoadLevelInstance(World, MapPath, Position, Rotation, bOutSuccess, LevelName, LevelStreamingClass);

	if (!bOutSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Level loading failed for: %s"), *MapPath);
	}
}

// Called every frame
void AGenerateLevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGenerateLevelActor::GenerateGrid()
{
    if (!SubActorClass) return;

    FVector Origin = GetActorLocation();
    for (int32 x = 0; x < GridSizeX; ++x)
    {
        for (int32 y = 0; y < GridSizeY; ++y)
        {
            FVector Location = Origin + FVector(x * GridSpacing, y * GridSpacing, 0.f);
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SubActorClass, Location, FRotator::ZeroRotator, SpawnParams);

            if (SpawnedActor)
            {
                SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
            }
        }
    }
}