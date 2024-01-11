// Fill out your copyright notice in the Description page of Project Settings.


#include "ICRegenComponent.h"

UICRegenComponent::UICRegenComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UICRegenComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatComponent = Cast<UCombatComponent>(GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));

	if (!CombatComponent)
	{
		return;
	}

	CombatComponent->OnEnterCombat.AddDynamic(this, &UICRegenComponent::HandleEnterCombat);
	CombatComponent->OnOutOfCombat.AddDynamic(this, &UICRegenComponent::HandleExitCombat);

	if (!CombatComponent->bInCombat) {
		HandleExitCombat();
	}

}

void UICRegenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UICRegenComponent::UpdateStats(int32 ChangeInItemAmount)
{
	Super::UpdateStats(ChangeInItemAmount);
}

void UICRegenComponent::HandleEnterCombat()
{
	GetWorld()->GetTimerManager().SetTimer(CombatTimerHandle, this, &UICRegenComponent::HealPlayer, HealSpeedTime, true);
}

void UICRegenComponent::HandleExitCombat()
{
	GetWorld()->GetTimerManager().PauseTimer(CombatTimerHandle);
}

void UICRegenComponent::HealPlayer()
{
	// As ItemCount increases there will be less healing
	CombatComponent->ServerHeal(HealAmount / (1.4f + ItemCount));
}
