#include "EnemyManagerActor.h"
#include "GameFramework/Character.h"
#include "EnemySpawnNodeActor.h"
#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GenerateLevelActor.h"
#include "../../../../../../../Source/Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "LevelAdvancedFriendsGameInstance.h"

// Sets default values
AEnemyManagerActor::AEnemyManagerActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    EnemiesPerPlayer = 5; // Starting number of enemies per player
    WaveCount = 0; // Initial wave count
}

// Called when the game starts or when spawned
void AEnemyManagerActor::BeginPlay()
{
    Super::BeginPlay();

    AGenerateLevelActor* GenerateLevelActor = Cast<AGenerateLevelActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AGenerateLevelActor::StaticClass()));
    if (GenerateLevelActor) {
        GenerateLevelActor->OnLevelGenerateComplete.AddDynamic(this, &AEnemyManagerActor::BeginSpawning);
    }

    GetWorldTimerManager().SetTimer(UpdateGameTimerHandle, this, &AEnemyManagerActor::UpdateGameTimer, 0.1f, true);

    ULevelAdvancedFriendsGameInstance* LevelAdvancedFriendsGameInstance = Cast<ULevelAdvancedFriendsGameInstance>(GetWorld()->GetGameInstance());
    CurrentGameTime = LevelAdvancedFriendsGameInstance->CurrentGameTime;
}

void AEnemyManagerActor::BeginSpawning() 
{
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemyManagerActor::SpawnEnemies, SpawnInterval, true);
}

void AEnemyManagerActor::UpdateGameTimer() 
{
    CurrentGameTime += 0.1f;
    OnGameTimeUpdate.Broadcast(CurrentGameTime);
}

// Called every frame
void AEnemyManagerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemyManagerActor::SpawnEnemies()
{
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnNodeActor::StaticClass(), Actors);

    // Collect spawn nodes
    TArray<AEnemySpawnNodeActor*> EnemySpawnNodeActors;
    for (AActor* Actor : Actors)
    {
        if (AEnemySpawnNodeActor* EnemySpawnNodeActor = Cast<AEnemySpawnNodeActor>(Actor))
        {
            EnemySpawnNodeActors.Add(EnemySpawnNodeActor);
        }
    }

    if (EnemySpawnNodeActors.Num() < 4) {
        UE_LOG(LogTemp, Warning, TEXT("Invalid amount of spawn nodes: %d"), EnemySpawnNodeActors.Num());
        return;
    }

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player character not found"));
        return;
    }

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSys)
    {
        UE_LOG(LogTemp, Warning, TEXT("Navigation system not found"));
        return;
    }

    EnemySpawnNodeActors.Sort([&PlayerCharacter](const AEnemySpawnNodeActor& A, const AEnemySpawnNodeActor& B) {
        return FVector::Dist(PlayerCharacter->GetActorLocation(), A.GetActorLocation()) < FVector::Dist(PlayerCharacter->GetActorLocation(), B.GetActorLocation());
        });

    for (int32 i = 0; i < EnemiesPerPlayer; ++i)
    {
        for (AEnemySpawnNodeActor* SpawnNode : EnemySpawnNodeActors)
        {
            FNavLocation NavLocation;
            const float SpawnRadius = 500.0f; // Adjust radius as needed

            // Adjusted part: Project spawn location to the nearest navigable ground surface
            if (NavSys->GetRandomPointInNavigableRadius(SpawnNode->GetActorLocation(), SpawnRadius, NavLocation))
            {
                FActorSpawnParameters SpawnParameters;
                SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
                
                // Get the capsule component height of the EnemyCharacter
                float CapsuleHalfHeight = 0.0f;
                if (EnemyCharacter)
                {
                    // Assuming the default capsule component is used for the character's collision
                    const UCapsuleComponent* CapsuleComponent = EnemyCharacter->GetDefaultObject<ACharacter>()->GetCapsuleComponent();
                    if (CapsuleComponent)
                    {
                        CapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
                    }
                }

                // Modify the NavLocation's Z value by adding the capsule's half height
                FVector ModifiedLocation = NavLocation.Location;
                ModifiedLocation.Z += CapsuleHalfHeight * 2; // Multiply by 2 to get the full height

                // Now spawn the enemy character at the modified location
                ACharacter* SpawnedEnemy = GetWorld()->SpawnActor<ACharacter>(EnemyCharacter, FTransform(ModifiedLocation), SpawnParameters);

                if (SpawnedEnemy)
                {
                    break; // Successfully spawned an enemy, break to attempt the next spawn
                }
            }
        }
    }

    WaveCount++;
    if (WaveCount % EnemySpawnRateWaveIncrement == 0)
    {
        EnemiesPerPlayer += EnemyIncreaseAmount;
    }
}
