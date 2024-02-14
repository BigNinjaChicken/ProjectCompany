#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "EnemySpawnNodeActor.h"
#include "EnemyManagerActor.generated.h"

class AEnemySpawnNodeActor;
class ACharacter;

UCLASS()
class STEAMCOMPANY_API AEnemyManagerActor : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    AEnemyManagerActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
    void BeginSpawning();
public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY()
    FTimerHandle SpawnTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 EnemiesPerPlayer = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 CurrentLevel = 0;

    UPROPERTY()
    int32 WaveCount;

//     UPROPERTY()
//     TArray<AEnemySpawnNodeActor*> EnemySpawnNodeActors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<ACharacter> EnemyCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float SpawnInterval = 15.0f;

    UFUNCTION()
    void SpawnEnemies();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int EnemySpawnRateWaveIncrement = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int EnemyIncreaseAmount = 1;
};