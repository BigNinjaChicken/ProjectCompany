
#include "BossCharacter.h"
#include "CombatComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Engine/World.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "ItemPickupActor.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "ObjectiveSystemComponent.h"

ABossCharacter::ABossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Get the UCombatComponent from this actor
    UCombatComponent* CombatComponent = Cast<UCombatComponent>(GetComponentByClass(UCombatComponent::StaticClass()));
    if (CombatComponent)
    {
        // Bind to the FOnActorDead delegate
        CombatComponent->OnActorDead.AddDynamic(this, &ABossCharacter::HandleActorDead);
    }
}

void ABossCharacter::HandleActorDead()
{
    // Drop Item
    if (DropedItemPossibilites.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Owner is not ACharacter"));
        return;
    }

    if (bHasDroppedItem) {
        return;
    }
    
    int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, DropedItemPossibilites.Num() - 1);
    TSubclassOf<AItemPickupActor> ItemActorToSpawn = DropedItemPossibilites[RandomIndex];
    if (ItemActorToSpawn)
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AItemPickupActor* SpawnedActor = GetWorld()->SpawnActor<AItemPickupActor>(ItemActorToSpawn, GetActorLocation(), GetActorRotation(), SpawnParameters);
        bHasDroppedItem = true;
    }

    // Mark off objective
    TArray<AActor*> OutActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), OutActors);
    for (AActor* OutActor : OutActors) 
    {
        ACharacter* CharacterActor = Cast<ACharacter>(OutActor);
        if (CharacterActor) {
            UActorComponent* ActorComponent = CharacterActor->GetComponentByClass(UObjectiveSystemComponent::StaticClass());
            if (ActorComponent) {
                UObjectiveSystemComponent* ObjectiveSystemComponent = Cast<UObjectiveSystemComponent>(ActorComponent);
                ObjectiveSystemComponent->CompleteObjective(EObjectiveType::KillBoss);
            }
        }
    }

    OnBossDead.Broadcast();
}

void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
