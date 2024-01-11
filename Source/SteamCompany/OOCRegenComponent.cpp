// Fill out your copyright notice in the Description page of Project Settings.


#include "OOCRegenComponent.h"

UOOCRegenComponent::UOOCRegenComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOOCRegenComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatComponent = Cast<UCombatComponent>(GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));

	if (!CombatComponent)
	{
		return;
	}

	CombatComponent->OnEnterCombat.AddDynamic(this, &UOOCRegenComponent::HandleEnterCombat);
	CombatComponent->OnOutOfCombat.AddDynamic(this, &UOOCRegenComponent::HandleExitCombat);

	if (!CombatComponent->bInCombat) {
		HandleExitCombat();
	}

}

void UOOCRegenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UOOCRegenComponent::UpdateStats(int32 ChangeInItemAmount)
{
	Super::UpdateStats(ChangeInItemAmount);
}

void UOOCRegenComponent::HandleEnterCombat()
{
	GetWorld()->GetTimerManager().PauseTimer(CombatTimerHandle);
}

void UOOCRegenComponent::HandleExitCombat()
{
	GetWorld()->GetTimerManager().SetTimer(CombatTimerHandle, this, &UOOCRegenComponent::HealPlayer, HealSpeedTime, true);
}

void UOOCRegenComponent::HealPlayer()
{
	// As ItemCount increases there will be less healing
	CombatComponent->ServerHeal(HealAmount / (1.4f + ItemCount));
}
