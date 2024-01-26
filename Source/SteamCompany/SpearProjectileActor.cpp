// Fill out your copyright notice in the Description page of Project Settings.


#include "SpearProjectileActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"

ASpearProjectileActor::ASpearProjectileActor()
{
    PrimaryActorTick.bCanEverTick = true;

    RootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootCapsuleComponent"));
    RootComponent = RootCapsuleComponent;
    ArrowStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowStaticMesh"));
    ArrowStaticMesh->SetupAttachment(RootComponent);
    ArrowTipBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ArrowTipBoxComponent"));
    ArrowTipBoxComponent->SetupAttachment(ArrowStaticMesh);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->UpdatedComponent = ArrowStaticMesh; // Setting the component to be updated by the projectile movement
}

// Called when the game starts or when spawned
void ASpearProjectileActor::BeginPlay()
{
	Super::BeginPlay();

    ArrowTipBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpearProjectileActor::OnArrowTipOverlap);

}

// Called every frame
void ASpearProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpearProjectileActor::OnArrowTipOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
   // ProjectileMovementComponent->StopMovementImmediately();
}

