// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemStackComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Actor.h"

// Sets default values for this component's properties
UItemStackComponent::UItemStackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemStackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UItemStackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UItemStackComponent::AddItemEffect(TSubclassOf<UItemEffectComponent> ItemType)
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Owning Actor"));
        return;
    }

    UItemEffectComponent* ItemEffectComponent = Cast<UItemEffectComponent>(OwnerActor->GetComponentByClass(ItemType));
    if (!ItemEffectComponent)
    {
        FTransform ComponentTransform;
        ItemEffectComponent = Cast<UItemEffectComponent>(OwnerActor->AddComponentByClass(ItemType, false, ComponentTransform, false));
        if (ItemEffectComponent)
        {
            ItemEffectComponent->ItemCount = 1;
        }
    }
    else
    {
        ItemEffectComponent->ItemCount++;
    }

    if (ItemEffectComponent) {
        ItemEffectComponent->UpdateStats(1);
    }

}
