
#include "ItemStackComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "LevelAdvancedFriendsGameInstance.h"
#include "Starter/SteamCompanyPlayerController.h"
#include "../Classes/AdvancedSessionsLibrary.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UItemStackComponent::UItemStackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    SetIsReplicatedByDefault(true);
}

void UItemStackComponent::BeginPlay()
{
    Super::BeginPlay();

    ULevelAdvancedFriendsGameInstance* GameInstance = Cast<ULevelAdvancedFriendsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameInstance is not ULevelAdvancedFriendsGameInstance"));
        return;
    }

    ASteamCompanyPlayerController* PlayerController = Cast<ASteamCompanyPlayerController>(Cast<APawn>(GetOwner())->GetController());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController is not ASteamCompanyPlayerController"));
        return;
    }

    FBPUniqueNetId UniqueID;
    UAdvancedSessionsLibrary::GetUniqueNetID(PlayerController, UniqueID);

    FString PlayerName;
    UAdvancedSessionsLibrary::UniqueNetIdToString(UniqueID, PlayerName);

    if (PlayerName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerName is empty"));
        return;
    }

    if (GameInstance->PlayerItemsMap.Contains(PlayerName))
    {
        FItemEffectComponentArray ItemEffectComponentArray = GameInstance->PlayerItemsMap[PlayerName];
        for (const FItemData& PlayerItem : ItemEffectComponentArray.PlayerItems)
        {
            AddItemEffect(PlayerItem.ItemEffectComponent, PlayerItem.ItemCount);
            UE_LOG(LogTemp, Warning, TEXT("Number of items in ItemEffectComponentArray: %d"), PlayerItem.ItemCount);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerName not found in PlayerItemsMap"));
    }
}

void UItemStackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UItemStackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UItemStackComponent::AddItemEffect(TSubclassOf<UItemEffectComponent> ItemType, int32 ItemCount)
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
            ItemEffectComponent->ItemCount = ItemCount;
        }
    }
    else
    {
        ItemEffectComponent->ItemCount++;
    }

    if (ItemEffectComponent) {
        ItemEffectComponent->UpdateStats(ItemCount);
    }

    OnUpdateCurrentItems.Broadcast();
}
