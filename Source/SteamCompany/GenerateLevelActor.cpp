// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateLevelActor.h"
#include "TileActor.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "LevelInstance/LevelInstanceLevelStreaming.h"
// Include necessary headers
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

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

	if (HasAuthority())
	{
		ServerOnBeginGenerateLevel();
	}
}

void AGenerateLevelActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGenerateLevelActor::ServerOnBeginGenerateLevel_Implementation()
{
	MulticastOnBeginGenerateLevel();
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
			LevelIndex = FMath::RandRange(0, StraightLevels.Num() - 1);
			MulticastLoadLevelAtPosition(World, StraightLevels[LevelIndex], Position, Rotation);
			break;
		case ETileShape::Corner:
			// Handle Corner logic
			LevelIndex = FMath::RandRange(0, CornerLevels.Num() - 1);
			MulticastLoadLevelAtPosition(World, CornerLevels[LevelIndex], Position, Rotation);
			break;
		case ETileShape::ThreeWayIntersection:
			// Handle ThreeWayIntersection logic
			LevelIndex = FMath::RandRange(0, ThreeWayIntersectionLevels.Num() - 1);
			MulticastLoadLevelAtPosition(World, ThreeWayIntersectionLevels[LevelIndex], Position, Rotation);
			break;
		case ETileShape::FourWayIntersection:
			// Handle FourWayIntersection logic
			LevelIndex = FMath::RandRange(0, FourWayIntersectionLevels.Num() - 1);
			MulticastLoadLevelAtPosition(World, FourWayIntersectionLevels[LevelIndex], Position, Rotation);
			break;
		case ETileShape::EndCap:;
			// Handle EndCap logic
			LevelIndex = FMath::RandRange(0, EndCapLevels.Num() - 1);
			MulticastLoadLevelAtPosition(World, EndCapLevels[LevelIndex], Position, Rotation);
			break;
		default:
			// Handle default case
			break;
		}
	}
}

// Function to load a level into the current level at a specified position and rotation
void AGenerateLevelActor::MulticastLoadLevelAtPosition_Implementation(UWorld* World, const TSoftObjectPtr<UWorld>& MapAsset, const FVector& Position, const FRotator& Rotation)
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
	GetWorld()->UpdateLevelStreaming();
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