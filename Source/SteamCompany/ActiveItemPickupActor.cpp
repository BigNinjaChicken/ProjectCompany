// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveItemPickupActor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "PlayerInteractComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/SphereComponent.h"
#include "PlayerInteractableObjComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h"
#include "CombatComponent.h"
#include "ActiveItemSystemComponent.h"
#include "ObjectiveSystemComponent.h"

// Sets default values
AActiveItemPickupActor::AActiveItemPickupActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;

    InteractableBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBoxComponent"));
    SetRootComponent(InteractableBoxComponent);
    InteractableWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractableWidgetComponent"));
    InteractableWidgetComponent->SetupAttachment(GetRootComponent());
    DisplayRangeSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DisplayRangeSphereComponent"));
    DisplayRangeSphereComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AActiveItemPickupActor::BeginPlay()
{
    Super::BeginPlay();
    UPlayerInteractableObjComponent* PlayerInteractableObjComponent = Cast<UPlayerInteractableObjComponent>(GetComponentByClass(UPlayerInteractableObjComponent::StaticClass()));
    if (PlayerInteractableObjComponent)
    {
        // Bind to the FOnActorDead delegate
        PlayerInteractableObjComponent->OnInteract.AddDynamic(this, &AActiveItemPickupActor::OnInteractHandler);

    }

    InteractableWidgetComponent->SetHiddenInGame(true);
    DisplayRangeSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AActiveItemPickupActor::OnOverlapBegin);
    DisplayRangeSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AActiveItemPickupActor::OnOverlapEnd);
}

// Called every frame
void AActiveItemPickupActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AActiveItemPickupActor::OnInteractHandler(ACharacter* InteractingPlayer)
{
    if (!InteractingPlayer)
    {
        return;
    }

    UCombatComponent* OtherCombatComponent = Cast<UCombatComponent>(InteractingPlayer->GetComponentByClass(UCombatComponent::StaticClass()));
    if (!OtherCombatComponent || !OtherCombatComponent->bIsPlayer)
    {
        return;
    }

    UActiveItemSystemComponent* ActiveItemSystemComponent = Cast<UActiveItemSystemComponent>(InteractingPlayer->GetComponentByClass(UActiveItemSystemComponent::StaticClass()));
    if (ActiveItemSystemComponent)
    {
        ActiveItemSystemComponent->AddItemEffect(ItemEffectType);
    }

    Destroy();
}


void AActiveItemPickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    InteractableWidgetComponent->SetHiddenInGame(false);
}

void AActiveItemPickupActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    InteractableWidgetComponent->SetHiddenInGame(true);
}