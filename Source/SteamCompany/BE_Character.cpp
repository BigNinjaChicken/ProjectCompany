#include "BE_Character.h"
#include "Kismet/GameplayStatics.h"
#include "LearningAgentsManager.h"

// Sets default values
ABE_Character::ABE_Character()
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABE_Character::BeginPlay()
{
	Super::BeginPlay();

	ALearningAgentsManager* LearningAgentsManager = Cast<ALearningAgentsManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ALearningAgentsManager::StaticClass()));
	if (!LearningAgentsManager) {
		UE_LOG(LogTemp, Warning, TEXT("Could not find LearningAgentsManager. Add one to the scene."));
		return;
	}

	AgentId = LearningAgentsManager->AddAgent(this);
}

// Called every frame
void ABE_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABE_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
