// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveItemSystemComponent.h"
#include "ActiveItemsUserWidget.h"
#include "ActiveItemEffectComponent.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Components/GridPanel.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "ItemAbilityUserWidget.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Components/GridSlot.h"

UActiveItemSystemComponent::UActiveItemSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UActiveItemSystemComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UActiveItemSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UActiveItemSystemComponent::AddItemEffect(TSubclassOf<UActiveItemEffectComponent> ItemType)
{
    if (!IsActive()) {
        UE_LOG(LogTemp, Warning, TEXT("ActiveItemSystemComponent Is Not Active"));
        return;
    }

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Owning Actor"));
        return;
    }

    FTransform ComponentTransform;
    UActiveItemEffectComponent* ItemEffectComponent = Cast<UActiveItemEffectComponent>(OwnerCharacter->AddComponentByClass(ItemType, false, ComponentTransform, false));
    if (!ItemEffectComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("ActiveItemEffectComponent null"));
        return;
    }

    UGridPanel* ItemGridPanel = ActiveItemWidget->ItemGridPanel;
    if (!ItemGridPanel)
    {
        UE_LOG(LogTemp, Warning, TEXT("ActiveItemEffectComponent null"));
        return;
    }

    UItemAbilityUserWidget* ItemAbilityUserWidget = CreateWidget<UItemAbilityUserWidget>(GetWorld(), ItemAbilityWidgetClass);
    ItemAbilityUserWidget->NameText = FText::FromString(ItemEffectComponent->ItemName);
    ItemAbilityUserWidget->ButtonText = FText::FromString(ItemEffectComponent->Button);

    ItemEffectComponent->ItemAbilityUserWidget = ItemAbilityUserWidget;

    UGridSlot* GridSlot = Cast<UGridSlot>(ItemGridPanel->AddChild(ItemAbilityUserWidget));
    GridSlot->SetRow(itemCount / 4);
    GridSlot->SetColumn(itemCount % 4);
    itemCount++;

    OnUpdateActiveCurrentItems.Broadcast(ItemEffectComponent);
}

void UActiveItemSystemComponent::Activate(bool bReset)
{
    Super::Activate();

    // Check if the Widget class is set in the Blueprint
    if (!ActiveItemsWidgetClass)
    {
        return;
    }

    ActiveItemWidget = CreateWidget<UActiveItemsUserWidget>(GetWorld(), ActiveItemsWidgetClass);

    if (!ActiveItemWidget)
    {
        return;
    }

    ActiveItemWidget->AddToViewport();
}