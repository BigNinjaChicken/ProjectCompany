
#include "BE_LearningAgentsManager.h"

ABE_LearningAgentsManager::ABE_LearningAgentsManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABE_LearningAgentsManager::BeginPlay()
{
	Super::BeginPlay();
}

void ABE_LearningAgentsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}