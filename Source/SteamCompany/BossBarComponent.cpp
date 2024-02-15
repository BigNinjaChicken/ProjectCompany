// Fill out your copyright notice in the Description page of Project Settings.


#include "BossBarComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BossCharacter.h"
#include "CombatComponent.h"
#include "BossEventActor.h"

// Sets default values for this component's properties
UBossBarComponent::UBossBarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossBarComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UBossBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* BossActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABossCharacter::StaticClass());
	if (!BossActor) {
		return;
	}

	ABossCharacter* BossCharacter = Cast<ABossCharacter>(BossActor);
	if (!BossCharacter) {
		return;
	}

	UActorComponent* BossComponent = BossCharacter->GetComponentByClass(UCombatComponent::StaticClass());
	BossCombatComponent = Cast<UCombatComponent>(BossComponent);
	BossCombatComponent->OnActorDead.AddDynamic(this, &UBossBarComponent::BossDead);
	BossCombatComponent->OnTakeDamageComplete.AddDynamic(this, &UBossBarComponent::BossTakeDamage);

	if (bBossSpawned) {
		BossSpawned();
		bBossSpawned = true;
	}
}

void UBossBarComponent::BossSpawned()
{
	if (BossCombatComponent) {
		OnBossSpawned.Broadcast(BossCombatComponent);
	}
}

void UBossBarComponent::BossTakeDamage()
{
	if (BossCombatComponent) {
		OnBossTakeDamage.Broadcast(BossCombatComponent);
	}
}

void UBossBarComponent::BossDead()
{
	OnBossDead.Broadcast();
	bBossSpawned = false;
}

