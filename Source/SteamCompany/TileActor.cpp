// Fill out your copyright notice in the Description page of Project Settings.


#include "TileActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ATileActor::ATileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(Root);
	ChurchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChurchMesh"));
	ChurchMesh->SetupAttachment(TileMesh);
	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	ForwardArrow->SetupAttachment(TileMesh);
	LeftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArrow"));
	LeftArrow->SetupAttachment(TileMesh);
	RightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrow"));
	RightArrow->SetupAttachment(TileMesh);
	BackwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("BackwardArrow"));
	BackwardArrow->SetupAttachment(TileMesh);

}

// Called when the game starts or when spawned
void ATileActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATileActor::OnTileShapeChanged()
{
	// Implement logic based on the TileShape
	ForwardArrow->SetVisibility(true);
	BackwardArrow->SetVisibility(true);
	LeftArrow->SetVisibility(true);
	RightArrow->SetVisibility(true);
	ChurchMesh->SetVisibility(false);
	SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));

	if (bIsChurch) ChurchMesh->SetVisibility(true);

	switch (TileShape)
	{
	case ETileShape::Straight:
		// Handle Straight logic
		LeftArrow->SetVisibility(false);
		RightArrow->SetVisibility(false);
		break;
	case ETileShape::Corner:
		// Handle Corner logic
		LeftArrow->SetVisibility(false);
		ForwardArrow->SetVisibility(false);
		break;
	case ETileShape::ThreeWayIntersection:
		// Handle ThreeWayIntersection logic
		ForwardArrow->SetVisibility(false);
		break;
	case ETileShape::FourWayIntersection:
		// Handle FourWayIntersection logic
		break;
	case ETileShape::BigFourWayIntersection:
		// Handle BigFourWayIntersection logic
		SetActorScale3D(FVector(2.0f, 2.0f, 1.0f));
		break;
	case ETileShape::EndCap:
		ForwardArrow->SetVisibility(false);
		LeftArrow->SetVisibility(false);
		RightArrow->SetVisibility(false);
		// Handle EndCap logic
		break;
	default:
		// Handle default case
		break;
	}
}

#if WITH_EDITOR
void ATileActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Check if the TileShape property was the one changed
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ATileActor, TileShape))
	{
		OnTileShapeChanged();
	}
}
#endif