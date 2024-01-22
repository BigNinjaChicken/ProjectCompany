// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnNodeActor.h"

// Sets default values
AEnemySpawnNodeActor::AEnemySpawnNodeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawnNodeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawnNodeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

