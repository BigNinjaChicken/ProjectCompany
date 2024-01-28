// Fill out your copyright notice in the Description page of Project Settings.


#include "SpearProjectileActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "CombatComponent.h"

ASpearProjectileActor::ASpearProjectileActor()
{
    PrimaryActorTick.bCanEverTick = true;

    RootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootCapsuleComponent"));
    RootComponent = RootCapsuleComponent;
    ArrowStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowStaticMesh"));
    ArrowStaticMesh->SetupAttachment(RootComponent);
    ArrowTipBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ArrowTipBoxComponent"));
    ArrowTipBoxComponent->SetupAttachment(ArrowStaticMesh);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    
}

// Called when the game starts or when spawned
void ASpearProjectileActor::BeginPlay()
{
	Super::BeginPlay();

    ArrowTipBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpearProjectileActor::OnArrowTipOverlap);
    ProjectileMovementComponent->UpdatedComponent = RootComponent;

}

// Called every frame
void ASpearProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpearProjectileActor::OnArrowTipOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Log the name of the other actor
    if (OtherActor->IsA(ASpearProjectileActor::StaticClass())) return;

    if (OtherActor == GetOwner()) return;

    UCombatComponent* OwnerCombatComponent = Cast<UCombatComponent>(GetOwner()->GetComponentByClass(UCombatComponent::StaticClass()));
    UCombatComponent* OtherCombatComponent = Cast<UCombatComponent>(OtherActor->GetComponentByClass(UCombatComponent::StaticClass()));
    if (OtherCombatComponent) {
        if ((OtherCombatComponent->bIsPlayer && !OwnerCombatComponent->bIsPlayer) || 
            (!OtherCombatComponent->bIsPlayer && OwnerCombatComponent->bIsPlayer)) {
            ServerRequestDamage(OtherCombatComponent, Damage);
            Destroy();
        }
    }

    if (OtherCombatComponent) return;

    ProjectileMovementComponent->StopMovementImmediately();
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

    ArrowTipBoxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ASpearProjectileActor::OnArrowTipOverlap);

    OnSpearInWall.Broadcast();
}

void ASpearProjectileActor::ServerRequestDamage_Implementation(UCombatComponent* OtherCombatComp, float DamageAmount)
{
    if (OtherCombatComp && OtherCombatComp->GetOwner()->HasAuthority())
    {
        OtherCombatComp->ServerTakeDamage(DamageAmount);
    }
}
