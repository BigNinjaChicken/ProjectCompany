#include "EnemyManagerActor.h"
#include "GameFramework/Character.h"
#include "EnemySpawnNodeActor.h"
#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GenerateLevelActor.h"

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
}

void AEnemyManagerActor::BeginSpawning() 
{
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemyManagerActor::SpawnEnemies, SpawnInterval, true);
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

    TArray<AActor*> CharacterActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), CharacterActors);

    for (AActor* CharactorActor : CharacterActors)
    {
        ACharacter* Character = Cast<ACharacter>(CharactorActor);
        UActorComponent* CharacterComponent = Character->GetComponentByClass(UCombatComponent::StaticClass());
        UCombatComponent* CombatComponent = Cast<UCombatComponent>(CharacterComponent);

        if (!CombatComponent)
        {
            continue;
        }

        if (!CombatComponent->bIsPlayer)
        {
            continue;
        }

        // Sort the spawn nodes based on distance from the player
        EnemySpawnNodeActors.Sort([CharactorActor](const AEnemySpawnNodeActor& A, const AEnemySpawnNodeActor& B) {
            return FVector::Dist(CharactorActor->GetActorLocation(), A.GetActorLocation()) < FVector::Dist(CharactorActor->GetActorLocation(), B.GetActorLocation());
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

    WaveCount++;
    
    if (WaveCount % EnemySpawnRateWaveIncrement == 0)
    {
        EnemiesPerPlayer += EnemyIncreaseAmount;
    }
}