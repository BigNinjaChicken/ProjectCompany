
#include "IncreaseMaxHealthComponent.h"

UIncreaseMaxHealthComponent::UIncreaseMaxHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UIncreaseMaxHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatComponent = Cast<UCombatComponent>(GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));
}


// Called every frame
void UIncreaseMaxHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UIncreaseMaxHealthComponent::UpdateStats(int32 ChangeInItemAmount)
{
	Super::UpdateStats(ChangeInItemAmount);

	CombatComponent->MaxHealth += ChangeInItemAmount * 0.3;
}

