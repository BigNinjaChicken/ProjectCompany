// Fill out your copyright notice in the Description page of Project Settings.

#include "GenerateComponent.h"
#include "TileActor.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "LevelInstance/LevelInstanceLevelStreaming.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "GameFramework/PlayerController.h"
#include "../../../../../../../Source/Runtime/NavigationSystem/Public/NavMesh/NavMeshBoundsVolume.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../../../../../../../Source/Runtime/Core/Public/GenericPlatform/GenericPlatformMath.h"
#include "LevelAdvancedFriendsGameInstance.h"

UGenerateComponent::UGenerateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGenerateComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority()) OnBeginGenerateLevel();
}


// Called every frame
void UGenerateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGenerateComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void UGenerateComponent::OnBeginGenerateLevel()
{
	TArray<AActor*> ChildActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATileActor::StaticClass(), ChildActors);

	ULevelAdvancedFriendsGameInstance* GameInstance = Cast<ULevelAdvancedFriendsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	int32 Seed = (GameInstance->CurrentLevel + SeedOffset) * 100;
	
	for (AActor* ChildActor : ChildActors) {
		ATileActor* TileActor = Cast<ATileActor>(ChildActor);
		if (!TileActor) continue;

		UWorld* World = GetWorld();

		FVector Position = TileActor->GetActorLocation();
		Position.X *= ScaleFactor;
		Position.Y *= ScaleFactor;
		FRotator Rotation = TileActor->GetActorRotation();

		FRandomStream RandomStream(Seed);
		switch (TileActor->TileShape)
		{
		case ETileShape::Straight:
			// Handle Straight logic
			if (TileActor->bIsChurch) {
				LevelIndex = RandomStream.RandRange(0, ChurchStraightLevels.Num() - 1);
				LoadLevelAtPosition(World, ChurchStraightLevels[LevelIndex], Position, Rotation);
			}
			else {
				LevelIndex = RandomStream.RandRange(0, StraightLevels.Num() - 1);
				LoadLevelAtPosition(World, StraightLevels[LevelIndex], Position, Rotation);
			}
			break;
		case ETileShape::Corner:
			// Handle Corner logic
			if (TileActor->bIsChurch) {
				LevelIndex = RandomStream.RandRange(0, ChurchCornerLevels.Num() - 1);
				LoadLevelAtPosition(World, ChurchCornerLevels[LevelIndex], Position, Rotation);
			}
			else {
				LevelIndex = RandomStream.RandRange(0, CornerLevels.Num() - 1);
				LoadLevelAtPosition(World, CornerLevels[LevelIndex], Position, Rotation);
			}
			break;
		case ETileShape::ThreeWayIntersection:
			if (TileActor->bIsChurch) {
				LevelIndex = RandomStream.RandRange(0, ChurchThreeWayIntersectionLevels.Num() - 1);
				LoadLevelAtPosition(World, ChurchThreeWayIntersectionLevels[LevelIndex], Position, Rotation);
			}
			else {
				LevelIndex = RandomStream.RandRange(0, ThreeWayIntersectionLevels.Num() - 1);
				LoadLevelAtPosition(World, ThreeWayIntersectionLevels[LevelIndex], Position, Rotation);
			}
			break;
		case ETileShape::FourWayIntersection:
			// Handle FourWayIntersection logic
			if (TileActor->bIsChurch) {
				LevelIndex = RandomStream.RandRange(0, ChurchFourWayIntersectionLevels.Num() - 1);
				LoadLevelAtPosition(World, ChurchFourWayIntersectionLevels[LevelIndex], Position, Rotation);
			}
			else {
				LevelIndex = RandomStream.RandRange(0, FourWayIntersectionLevels.Num() - 1);
				LoadLevelAtPosition(World, FourWayIntersectionLevels[LevelIndex], Position, Rotation);
			}
			break;
		case ETileShape::EndCap:;
			// Handle EndCap logic
			if (TileActor->bIsChurch) {
				LevelIndex = RandomStream.RandRange(0, ChurchEndCapLevels.Num() - 1);
				LoadLevelAtPosition(World, ChurchEndCapLevels[LevelIndex], Position, Rotation);
			}
			else {
				LevelIndex = RandomStream.RandRange(0, EndCapLevels.Num() - 1);
				LoadLevelAtPosition(World, EndCapLevels[LevelIndex], Position, Rotation);
			}
			break;
		default:
			// Handle default case
			break;
		}
	}
}

// Function to load a level into the current level at a specified position and rotation _Implementation
void UGenerateComponent::LoadLevelAtPosition(UWorld* World, const TSoftObjectPtr<UWorld>& MapAsset, const FVector& Position, const FRotator& Rotation)
{
	if (MapAsset.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("MapAsset is null"));
		return;
	}

	if (!World) {
		UE_LOG(LogTemp, Warning, TEXT("World is null"));
		return;
	}

	FString MapPath = MapAsset.GetLongPackageName();
	bool bOutSuccess;

	// Ensure LevelName is unique for each function call
	FString LevelName = "LevelTile" + FString::FromInt(LevelNameIndex);
	LevelNameIndex++;  // Increment index to ensure uniqueness for the next call

	// Append a unique identifier, like current timestamp, to LevelName
	FString UniqueSuffix = FDateTime::Now().ToString(TEXT("%Y%m%d%H%M%S%ms"));
	LevelName += "_" + UniqueSuffix;

	ULevelStreamingDynamic* Level = ULevelStreamingDynamic::LoadLevelInstance(World, MapPath, Position, Rotation, bOutSuccess, LevelName, LevelStreamingClass);

	if (!bOutSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Level loading failed for: %s"), *MapPath);
	}
}