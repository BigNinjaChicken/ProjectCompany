
#include "ItemStackComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

UItemStackComponent::UItemStackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    SetIsReplicatedByDefault(true);
}

void UItemStackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UItemStackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UItemStackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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

    OnUpdateCurrentItems.Broadcast();
}
