// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopActor.h"
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
#include "ShopSystemComponent.h"

// Sets default values
AShopActor::AShopActor()
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
void AShopActor::BeginPlay()
{
    Super::BeginPlay();
    UPlayerInteractableObjComponent* PlayerInteractableObjComponent = Cast<UPlayerInteractableObjComponent>(GetComponentByClass(UPlayerInteractableObjComponent::StaticClass()));
    if (PlayerInteractableObjComponent)
    {
        // Bind to the FOnActorDead delegate
        PlayerInteractableObjComponent->OnInteract.AddDynamic(this, &AShopActor::OnInteractHandler);
    }

    InteractableWidgetComponent->SetHiddenInGame(true);
    DisplayRangeSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AShopActor::OnOverlapBegin);
    DisplayRangeSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AShopActor::OnOverlapEnd);
}

// Called every frame
void AShopActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
// 
// UE_LOG(LogTemp, Warning, TEXT("CombatComponent null"));
// return;
void AShopActor::OnInteractHandler()
{
    ULevelAdvancedFriendsGameInstance* AdvancedFriendsGameInstance = Cast<ULevelAdvancedFriendsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    TArray<AActor*> CharacterActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), CharacterActors);

    AActor* ClosestCharacter = nullptr;
    float MinDistance = FLT_MAX;

    for (AActor* CharacterActor : CharacterActors)
    {
        UCombatComponent* CombatComponent = Cast<UCombatComponent>(CharacterActor->GetComponentByClass(UCombatComponent::StaticClass()));
        if (CombatComponent && CombatComponent->bIsPlayer)
        {
            float Distance = FVector::Dist(CharacterActor->GetActorLocation(), this->GetActorLocation());
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                ClosestCharacter = CharacterActor;
            }
        }
    }

    if (ClosestCharacter)
    {
        UShopSystemComponent* ShopSystemComponent = Cast<UShopSystemComponent>(ClosestCharacter->GetComponentByClass(UShopSystemComponent::StaticClass()));
        if (ShopSystemComponent)
        {
            ShopSystemComponent->Interact();
        }
    }
}


void AShopActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    InteractableWidgetComponent->SetHiddenInGame(false);
}

void AShopActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    InteractableWidgetComponent->SetHiddenInGame(true);
}

