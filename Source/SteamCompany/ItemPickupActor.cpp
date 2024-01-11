// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickupActor.h"
#include "Components/MeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ItemStackComponent.h"
#include "CombatComponent.h"

// Sets default values
AItemPickupActor::AItemPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBoxCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBoxCapsuleComponent"));
	SetRootComponent(HitBoxCapsuleComponent);
	ItemModelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemModelMeshComponent"));
	ItemModelMeshComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItemPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
	HitBoxCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemPickupActor::OnOverlapBegin);
}

// Called every frame
void AItemPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemPickupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	UCombatComponent* OtherCombatComponent = Cast<UCombatComponent>(OtherActor->GetComponentByClass(UCombatComponent::StaticClass()));
	if (!OtherCombatComponent->bIsPlayer) {
		return;
	}

	UItemStackComponent* ItemStackComponent = Cast<UItemStackComponent>(OtherActor->GetComponentByClass(UItemStackComponent::StaticClass()));
	if (ItemStackComponent)
	{
		ItemStackComponent->AddItemEffect(ItemType);
	}
	
	Destroy();
}