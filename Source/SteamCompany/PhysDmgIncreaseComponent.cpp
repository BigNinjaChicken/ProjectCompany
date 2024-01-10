
#include "PhysDmgIncreaseComponent.h"

UPhysDmgIncreaseComponent::UPhysDmgIncreaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPhysDmgIncreaseComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatComponent = Cast<UCombatComponent>(GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));
}


// Called every frame
void UPhysDmgIncreaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPhysDmgIncreaseComponent::UpdateStats(int32 ChangeInItemAmount)
{
	Super::UpdateStats(ChangeInItemAmount);

	CombatComponent->StrengthMultiplier += ChangeInItemAmount * 0.3;
}

