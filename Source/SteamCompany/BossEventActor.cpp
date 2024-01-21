// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEventActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Engine/World.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Actor.h"

// Sets default values
ABossEventActor::ABossEventActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	BossArenaSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("BossArenaSphereComponent"));
	SetRootComponent(BossArenaSphereComponent);
	BossArenaStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossArenaStaticMeshComponent"));
	BossArenaStaticMeshComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABossEventActor::BeginPlay()
{
	Super::BeginPlay();

	OnBeginBossEvent.AddDynamic(this, &ABossEventActor::SpawnBossEvent);
}

void ABossEventActor::BeginBossEvent()
{
	OnBeginBossEvent.Broadcast();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossEventActor::SpawnBoss, SpawnDelay, false);
}

void ABossEventActor::SpawnBoss()
{
	if (!BossCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Chosen Boss Character"));
		return;
	}

	FTransform SpawnTransform = GetActorTransform();
	ABossCharacter* SpawnedBossCharacter = GetWorld()->SpawnActor<ABossCharacter>(BossCharacter, SpawnTransform);

	if (!SpawnedBossCharacter) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn boss"));
		return;
	}

	SpawnedBossCharacter->OnBossDead.AddDynamic(this, &ABossEventActor::OnBossDeadHandler);
}

void ABossEventActor::OnBossDeadHandler()
{
	if (!BossPortalActorClass) {
		UE_LOG(LogTemp, Warning, TEXT("No BossPortalActorClass"));
		return;
	}
	ABossPortalActor* BossPortalActor = GetWorld()->SpawnActor<ABossPortalActor>(BossPortalActorClass, BossPortalSpawnTransform * GetActorTransform());
}

// Called every frame
void ABossEventActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

