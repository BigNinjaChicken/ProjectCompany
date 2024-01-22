#include "EnemyManagerActor.h"
#include "GameFramework/Character.h"
#include "EnemySpawnNodeActor.h"
#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"

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

    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemyManagerActor::SpawnEnemies, SpawnInterval, true);

    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnNodeActor::StaticClass(), Actors);

    for (AActor* Actor : Actors)
    {
        if (AEnemySpawnNodeActor* EnemySpawnNodeActor = Cast<AEnemySpawnNodeActor>(Actor))
        {
            EnemySpawnNodeActors.Add(EnemySpawnNodeActor);
        }
    }
}

// Called every frame
void AEnemyManagerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemyManagerActor::SpawnEnemies()
{
    TArray<AActor*> Players;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Players);

    for (AActor* Player : Players)
    {
        if (ACharacter* Character = Cast<ACharacter>(Player))
        {
            UActorComponent* CharacterComponent = Character->GetComponentByClass(UCombatComponent::StaticClass());
            if (UCombatComponent* CombatComponent = Cast<UCombatComponent>(CharacterComponent))
            {
                if (CombatComponent->bIsPlayer)
                {
                    // Sort the spawn nodes based on distance from the player
                    EnemySpawnNodeActors.Sort([Player](const AEnemySpawnNodeActor& A, const AEnemySpawnNodeActor& B) {
                        return FVector::Dist(Player->GetActorLocation(), A.GetActorLocation()) < FVector::Dist(Player->GetActorLocation(), B.GetActorLocation());
                        });

                    // Spawn enemies at the closest nodes
                    for (int32 i = 0; i < EnemiesPerPlayer; ++i)
                    {
                        int32 RandomIndex = FMath::RandRange(0, 4);
                        if (EnemySpawnNodeActors[RandomIndex])
                        {
                            FTransform SpawnTransform = EnemySpawnNodeActors[RandomIndex]->GetActorTransform();
                            ACharacter* SpawnedEnemyCharacter = GetWorld()->SpawnActor<ACharacter>(EnemyCharacter, SpawnTransform);
                        }
                    }
                }
            }
        }
    }

    WaveCount++;
    if (WaveCount % 3 == 0)
    {
        EnemiesPerPlayer++;
    }
}