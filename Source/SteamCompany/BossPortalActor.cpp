// Fill out your copyright notice in the Description page of Project Settings.


#include "BossPortalActor.h"
#include "PlayerInteractableObjComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h"
#include "LevelAdvancedFriendsGameInstance.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ItemEffectComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "CombatComponent.h"
#include "Starter/SteamCompanyPlayerController.h"
#include "../Classes/AdvancedSessionsLibrary.h"

// Sets default values
ABossPortalActor::ABossPortalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractableBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBoxComponent"));
	SetRootComponent(InteractableBoxComponent);
	InteractableWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractableWidgetComponent"));
	InteractableWidgetComponent->SetupAttachment(GetRootComponent());
	DisplayRangeSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DisplayRangeSphereComponent"));
	DisplayRangeSphereComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABossPortalActor::BeginPlay()
{
    Super::BeginPlay();
    UPlayerInteractableObjComponent* PlayerInteractableObjComponent = Cast<UPlayerInteractableObjComponent>(GetComponentByClass(UPlayerInteractableObjComponent::StaticClass()));
    if (PlayerInteractableObjComponent)
    {
        // Bind to the FOnActorDead delegate
        PlayerInteractableObjComponent->OnInteract.AddDynamic(this, &ABossPortalActor::OnInteractHandler);
    }

    InteractableWidgetComponent->SetHiddenInGame(true);
    DisplayRangeSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossPortalActor::OnOverlapBegin);
    DisplayRangeSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABossPortalActor::OnOverlapEnd);
}

// Called every frame
void ABossPortalActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
// 
// UE_LOG(LogTemp, Warning, TEXT("CombatComponent null"));
// return;
void ABossPortalActor::OnInteractHandler()
{
//     ULevelAdvancedFriendsGameInstance* AdvancedFriendsGameInstance = Cast<ULevelAdvancedFriendsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//     TArray<AActor*> CharacterActors;
//     UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), CharacterActors);
// 
//     for (AActor* CharacterActor : CharacterActors)
//     {
//         UCombatComponent* CombatComponent = Cast<UCombatComponent>(CharacterActor->GetComponentByClass(UCombatComponent::StaticClass()));
//         if (CombatComponent && CombatComponent->bIsPlayer) 
//         {
//             FString PlayerName;
//             ASteamCompanyPlayerController* PlayerController = Cast<ASteamCompanyPlayerController>(Cast<APawn>(CharacterActor)->GetController());
//             FBPUniqueNetId UniqueID;
//             UAdvancedSessionsLibrary::GetUniqueNetID(PlayerController, UniqueID);
//             UAdvancedSessionsLibrary::UniqueNetIdToString(UniqueID, PlayerName);
// 
//             TArray<UActorComponent*> ItemEffectActorComponents;
//             CharacterActor->GetComponents(UItemEffectComponent::StaticClass(), ItemEffectActorComponents);
// 
//             FItemEffectComponentArray ItemEffectComponentArray;
//             for (UActorComponent* ItemEffectActorComponent : ItemEffectActorComponents) {
//                 UItemEffectComponent* ItemEffectComponent = Cast<UItemEffectComponent>(ItemEffectActorComponent);
//                 if (ItemEffectComponent) {
//                     FItemData ItemData;
//                     ItemData.ItemEffectComponent = ItemEffectComponent->GetClass();
//                     ItemData.ItemCount = ItemEffectComponent->ItemCount;
//                     ItemEffectComponentArray.PlayerItems.Add(ItemData);
//                 }
//             }
// 
//             AdvancedFriendsGameInstance->PlayerItemsMap.Add(PlayerName, ItemEffectComponentArray);
//         }
//     }

    FString InUrl = "/Game/Frazier/Creative/LV_Lair?listen";
    UAdvancedSessionsLibrary::ServerTravel(GetWorld(), InUrl, false, false);
    Destroy();
}

void ABossPortalActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    InteractableWidgetComponent->SetHiddenInGame(false);
}

void ABossPortalActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    InteractableWidgetComponent->SetHiddenInGame(true);
}

